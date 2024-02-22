#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>


#define SUCCESS "SUCCESS"
#define ERROR "ERROR"
#define PIPE_IN "REQ_PIPE_92790"
#define PIPE_OUT "RESP_PIPE_92790"

#define PIPE_CREATE_ERR "cannot create the response pipe"
#define PIPE_IN_OPEN_ERR "cannot open the request pipe"
#define PIPE_OUT_OPEN_ERR "cannot open the response pipe"

#define START "START"
#define ECHO "ECHO"
#define CREATE_SHM "CREATE_SHM"
#define WRITE_TO_SHM "WRITE_TO_SHM"
#define MAPFILE "MAP_FILE"
#define READ_FROM_FILE_OFFSET "READ_FROM_FILE_OFFSET"
#define READ_FROM_FILE_SECTION "READ_FROM_FILE_SECTION"
#define READ_FROM_LOGICAL_SPACE_OFFSET "READ_FROM_LOGICAL_SPACE_OFFSET"
#define EXIT "EXIT"

#define MAX_STRING 250
#define SHM_NAME "/9ceOnR"

const int debug = 0;
const unsigned int variant = 92790;
const char separator = '!';
int fin = -1;
int fout = -1;
void* fMap = MAP_FAILED;
size_t fMapLen = 0;
int shmf = -1;
volatile void* shmaddr = NULL;
size_t shmlen = 0;

char comanda[MAX_STRING+1];
char sfield[MAX_STRING+1];
unsigned int ifield1;
unsigned int ifield2;
unsigned int ifield3;

typedef struct {
  int sect_type, sect_size, sect_offset;
  char sect_name[17];
}Section_Header;

int header_size = 0;
int nr_file_sect = 0;
Section_Header *section_headers = NULL;



bool parse()
{
  if (section_headers != NULL)
    return true; // e deja parsat

  if (fMap == MAP_FAILED) {
    if (debug)
    {
      printf("ERROR\n");
      printf("file not mapped\n");
    }
    return false;
  }

  // header are minim 7
  if (fMapLen < 7)
    return false;

  char magic[2];
  header_size = *(unsigned short*)(fMap + fMapLen - 4);
  *(unsigned short*)magic = *(unsigned short*)(fMap + fMapLen - 2);
  if (fMapLen < header_size || magic[0] != 'Z' || magic[1] != '5') {
    if (debug)
    {
      printf("ERROR\n");
      printf("wrong magic\n");
    }
    return false;
  }

  int version = *(unsigned short*)(fMap + fMapLen - header_size);
  nr_file_sect = *(unsigned char*)(fMap + fMapLen - header_size + 2);

  if (version < 13 || version>43)
  {
    if (debug)
    {
      printf("ERROR\n");
      printf("wrong version\n");
    }
    return false;
  }

  if (nr_file_sect < 3 || nr_file_sect>11)
  {
    if (debug)
    {
      printf("ERROR\n");
      printf("wrong sect_nr\n");
    }
    return false;
  }

  section_headers = (Section_Header*)malloc(nr_file_sect * sizeof(Section_Header));
  memset(section_headers, 0, nr_file_sect * sizeof(Section_Header));

  void* addr = fMap + fMapLen - header_size + 3;
  for (int i = 0; i < nr_file_sect; i++)
  {
    memcpy(section_headers[i].sect_name, addr, 16);
    addr += 16;
    section_headers[i].sect_type = *(unsigned short*)addr;
    addr += 4;
    section_headers[i].sect_offset = *(unsigned short*)addr;
    addr += 4;
    section_headers[i].sect_size = *(unsigned short*)addr;
    addr += 4;
  }

  int ok1 = 1;
  for (int i = 0; i < nr_file_sect; i++) {
    if (section_headers[i].sect_type != 18 && section_headers[i].sect_type != 40 && section_headers[i].sect_type != 48 && section_headers[i].sect_type != 43 && section_headers[i].sect_type != 88 && section_headers[i].sect_type != 14 && section_headers[i].sect_type != 81) {
      ok1 = 0;
      break;
    }
  }
  if (ok1 == 0)
  {
    if (debug)
    {
      printf("ERROR\n");
      printf("wrong sect_types\n");
    }
    return false;
  }

  if (debug) {
    printf("SUCCESS\n");
    printf("version=%d\n", version);
    printf("nr_sections=%d\n", nr_file_sect);
    for (int i = 0; i < nr_file_sect; i++) {
      printf("section%d: %s %d %d\n", i + 1, section_headers[i].sect_name, section_headers[i].sect_type, section_headers[i].sect_size);
    }
  }

  return true;
}

void printError(char* msg) {
  const size_t size = 250;
  char buf[size];
  snprintf(buf, size, "%s\n%s\n", ERROR, msg);
  perror(buf);
  exit(-1);
}

int readSField(char* buf) {
  char* c = buf;
  for (int i = 0; i < MAX_STRING; i++) {
    if (read(fin, c, sizeof(char)) == sizeof(char)) {
      if (*c == separator) {
        // ok, transforma in string normal
        *c = 0;
        if (debug) printf("read %s\n", buf);
        return 1;
      }
      c++;
    }
    else {
      // eroare citire pipe
      return 0;
    }
  }
  // string field > MAX_STRING
  return 0;
}

int readIField(unsigned int* field) {
  size_t count = read(fin, field, sizeof(unsigned int));
  return count == sizeof(unsigned int);
}

int writeSField(char* buf) {
  size_t len = strnlen(buf, MAX_STRING);
  strncpy(sfield, buf, len);
  sfield[len] = separator;
  if (len < MAX_STRING) {
    sfield[len + 1] = 0;
  }
  size_t count = write(fout, sfield, len+1);
  return count == len+1;
}

int writeIField(unsigned int field) {
  size_t count = write(fout, &field, sizeof(unsigned int));
  return count == sizeof(unsigned int);
}

void cleanup() {
  if (fin != -1) {
    close(fin);
  }
  if (fout != -1) {
    close(fout);
  }
  unlink(PIPE_OUT);
  if (shmaddr != NULL) {
    munmap((void*)shmaddr, shmlen);
    shmaddr = NULL;
    shmlen = 0;
  }
  if (shmf != -1) {
    close(shmf);
    shm_unlink(SHM_NAME);
    shmf = -1;
  }
  if (debug) printf("STOP\n");
}

void init() {
  if (mkfifo(PIPE_OUT, 0644)) {
    if (errno != EEXIST) {
      printError(PIPE_CREATE_ERR);
    }
  }

  fin = open(PIPE_IN, O_RDONLY);
  if (fin == -1) {
    cleanup();
    printError(PIPE_IN_OPEN_ERR);
  }

  fout = open(PIPE_OUT, O_WRONLY);
  if (fin == -1) {
    cleanup();
    printError(PIPE_OUT_OPEN_ERR);
  }

  writeSField(START);
  printf("%s\n", SUCCESS);
}

int doCreateSHM(unsigned int size) {
  int ret = 0;
  if (shmaddr != NULL) {
    ret = munmap((void*)shmaddr, shmlen);
    shmaddr = NULL;
  }
  if (ret == 0 && shmf != -1) {
    close(shmf);
    ret = shm_unlink(SHM_NAME);
    shmf = -1;
  }
  if (ret == 0) {
    shmf = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
    if (shmf != -1) {
      shmlen = size;
      ret = ftruncate(shmf, size);
      if (ret == 0) {
        shmaddr = (volatile void*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
        if (shmaddr == MAP_FAILED) {
          shmaddr = NULL;
          ret = -1;
        }
      }
    } else {
      ret = -1;
    }
  }
  return ret == 0;
}

int doWriteToSHM(unsigned int offset, unsigned int value) {
  int ret = 0;
  if (shmaddr != NULL && offset + sizeof(unsigned int) < shmlen) {
    volatile unsigned int* addr = (volatile unsigned int*)(shmaddr + offset);
    *addr = value;
    ret = 1;
  }
  return ret;
}

int doMapFile(char* name) {
  int ret = 0;
  int fd = -1;
  if (fMap != MAP_FAILED) {
    ret = munmap(fMap, fMapLen);
    fMap = MAP_FAILED;
    fMapLen = 0;
    if (section_headers != NULL)
      free(section_headers);
    section_headers = NULL;
    nr_file_sect = 0;
  }
  if (ret == 0) {
    fd = open(name, O_RDONLY);
  }
  if (ret == 0) {
    size_t size = (size_t)lseek(fd, 0, SEEK_END);
    if (size != -1) {
      fMap = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
      if (fMap != MAP_FAILED) {
        fMapLen = size;
      }
      else {
        fMap = MAP_FAILED;
        fMapLen = 0;
        ret = -1;
      }
    }
    else {
      ret = -1;
    }
  }
  return ret == 0;
}

int doReadFromFileOffset(unsigned int offset, unsigned int len) {
  int ret = 0;
  if (shmaddr != NULL && fMap != MAP_FAILED && offset + len <= shmlen && offset + len <= fMapLen) {
    void* addr = fMap + offset;
    memcpy((void* restrict)shmaddr, (void* restrict)addr, len);
    ret = 1;
  }
  return ret;
}

int doReadFromFileSection(unsigned int section, unsigned int offset, unsigned int len) {
  //TODO
  int ret = 0;
  if (parse()) {
    if (debug) printf("parse header size: %d sections: %d\n", header_size, nr_file_sect);
    if (0 < section && section <= nr_file_sect) {
      Section_Header* ps = &section_headers[section - 1];
      if (offset + len <= ps->sect_size) {
        if (debug) printf("section size: %d\n", ps->sect_size);
        ret = doReadFromFileOffset(ps->sect_offset + offset, len);
      }
    }
  }
  return ret;
}

int doReadFromLSO(unsigned int offset, unsigned int len) {
  //TODO
  return 0;
}

int writeResponse(char* cmd, int ok) {
  writeSField(cmd);
  ok = writeSField(ok ? SUCCESS : ERROR);
  return ok;
}

int executeCommand() {
  int ok = readSField(comanda);
  if (ok) {
    if (!strncmp(comanda, ECHO, MAX_STRING)) {
      writeSField("ECHO");
      writeSField("VARIANT");
      ok = writeIField(variant);
    } else if (!strncmp(comanda, CREATE_SHM, MAX_STRING)) {
      ok = readIField(&ifield1);
      if (ok) {
        ok = doCreateSHM(ifield1);
        ok = writeResponse(CREATE_SHM, ok);
      }
    } else if (!strncmp(comanda, WRITE_TO_SHM, MAX_STRING)) {
      ok = readIField(&ifield1) && readIField(&ifield2);
      if (ok) {
        ok = doWriteToSHM(ifield1, ifield2);
        ok = writeResponse(WRITE_TO_SHM, ok);
      }
    } else if (!strncmp(comanda, MAPFILE, MAX_STRING)) {
      ok = readSField(sfield);
      if (ok) {
        ok = doMapFile(sfield);
        ok = writeResponse(MAPFILE, ok);
      }
    } else if (!strncmp(comanda, READ_FROM_FILE_OFFSET, MAX_STRING)) {
      ok = readIField(&ifield1) && readIField(&ifield2);
      if (ok) {
        ok = doReadFromFileOffset(ifield1, ifield2);
        ok = writeResponse(READ_FROM_FILE_OFFSET, ok);
      }
    } else if (!strncmp(comanda, READ_FROM_FILE_SECTION, MAX_STRING)) {
      ok = readIField(&ifield1) && readIField(&ifield2) && readIField(&ifield3);
      if (ok) {
        ok = doReadFromFileSection(ifield1, ifield2, ifield3);
        ok = writeResponse(READ_FROM_FILE_SECTION, ok);
      }
    } else if (!strncmp(comanda, READ_FROM_LOGICAL_SPACE_OFFSET, MAX_STRING)) {
      ok = readIField(&ifield1) && readIField(&ifield2);
      if (ok) {
        ok = doReadFromLSO(ifield1, ifield2);
        ok = writeResponse(READ_FROM_LOGICAL_SPACE_OFFSET, ok);
      }
    } else if (!strncmp(comanda, EXIT, MAX_STRING)) {
      // iesire
      ok = 0;
    } else {
      // ignora comanda inexistenta
    }
  }
  return ok;
}


int main() {
  init();

  while (executeCommand()){
  }

  cleanup();
  return 0;
}
