#include <sys/types.h>
#include <dirent.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdbool.h>


typedef struct{
	int sect_type,sect_size,sect_offset;
	char sect_name[17];
}Section_Header;

int nr_file_sect=0;

Section_Header *section_headers=NULL;

void list(char *path)   
{
	DIR *dir=NULL;
	struct dirent *entry=NULL;
	char filePath[512];
	struct stat statBuf;
	
	dir=opendir(path);
	if(dir==NULL)
	{
		perror("ERROR");
		printf("\n");
		printf("invalid directory path");
		return;
	}
	else
	{
		printf("SUCCESS");
		printf("\n");
	}
	while((entry=readdir(dir)) !=NULL)
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			snprintf(filePath,512,"%s/%s",path,entry->d_name);
			if(lstat(filePath,&statBuf)==0)
			{
				 printf("%s\n",filePath);
			}
		}
	}
	closedir(dir);
}

void list1(char *path,char *lm)   
{
	DIR *dir=NULL;
	struct dirent *entry=NULL;
	char filePath[512];
	struct stat statBuf;
	
	dir=opendir(path);
	if(dir==NULL)
	{
		perror("ERROR");
		printf("\n");
		printf("invalid directory path");
		return;
	}
	else
	{
		printf("SUCCESS");
		printf("\n");
	}
	while((entry=readdir(dir)) !=NULL)
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			snprintf(filePath,512,"%s/%s",path,entry->d_name);
			if(lstat(filePath,&statBuf)==0)
			{
				 int l=strlen(lm);
	    			 int lp=strlen(filePath);
	    			 int x=lp-l;
	    			 int ok=0;
	    			 if(strcmp((filePath + x) , lm)==0)
	    			 {
	    				 ok = 1;
	    			 }
	    			 if(ok == 1)
	    				printf("%s\n",filePath);
			}
		}
	}
	closedir(dir);
}

int convert(long long n)
{
	long long k=0,nr=0,v[20];
	while(n!=0)
	{
		v[++k]=n%8;
		n=n/8;
	}
	for(int i=k;i>=1;i--)
		nr=nr*10+v[i];
	return nr;
}

void list_permisiuni(char *path,char *perm)   
{
	DIR *dir=NULL;
	struct dirent *entry=NULL;
	char filePath[512];
	struct stat statBuf;
	
	dir=opendir(path);
	if(dir==NULL)
	{
		perror("ERROR");
		printf("\n");
		printf("invalid directory path");
		return;
	}
	else
	{
		printf("SUCCESS");
		printf("\n");
	}
	while((entry=readdir(dir)) !=NULL)
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			snprintf(filePath,512,"%s/%s",path,entry->d_name);
			if(lstat(filePath,&statBuf)==0)
			{
			//in p am toate permisiunile fisierului selectat de path
	    			int p=statBuf.st_mode & 0777; 
	    			//printf("[%d]",p);
	    			int v1[4];
	    			v1[2]=p%10;
	    			p/=10;
	    			v1[1]=p%10;
	    			p/=10;
	    			v1[0]=p%10;
	    			int nr=0;
	    			for(int i=0;i<3;i++)
	    			{
	    				nr=nr*10+v1[i];
	    				
	    			}
	    			//printf("nr:%d",nr);
	    			int conv=convert(nr);
	    			//printf("convertit:%d ",conv);
	    			int v2[4];
	    			v2[2]=conv%10;
	    			conv/=10;
	    			v2[1]=conv%10;
	    			conv/=10;
	    			v2[0]=conv%10;
	    			/*for(int i=0;i<3;i++)
	    			{
	    				printf("%d ",v2[i]);
	    			}*/
	    			char p1[4],p2[4],p3[4];//pn[10],pnn[10];
	    			int v[4]={0};
		    			/*strncpy(p1,perm,3); //primele 3 litere  
		    			strcat(pn,perm+3);//sar peste primele 3 litere,mai am 6 litere in pn
		    			strncpy(p2,pn,3); //setul 2 de 3 litere
		    			strcat(pnn,p2+3);
		    			strncpy(p3,pnn,3);*/
	    			//int lp=strlen(perm);
	    			for(int i=0;i<3;i++)
	    			{
	    				p1[i]=perm[i];
	    				p2[i]=perm[i+3];
	    				p3[i]=perm[i+6];
	    				
	    			}
	    			p1[3]=0;
	    			p2[3]=0;
	    			p3[3]=0;
	    			//printf("%s %s %s\n",p1,p2,p3);
	    			if(strcmp(p1,"---")==0)
	    				v[0]=0;
	    			else
	    				if(strcmp(p1,"--x")==0)
	    					v[0]=1;
	    				else
	    					if(strcmp(p1,"-w-")==0)
	    						v[0]=2;
	    					else
	    						if(strcmp(p1,"-wx")==0)
	    							v[0]=3;
	    						else
	    							if(strcmp(p1,"r--")==0)
	    								v[0]=4;
	    							else
	    								if(strcmp(p1,"r-x")==0)
	    									v[0]=5;
	    								else
	    									if(strcmp(p1,"rw-")==0)
	    										v[0]=6;
	    									else
	    										if(strcmp(p1,"rwx")==0)
	    											v[0]=7;
	    			if(strcmp(p2,"---")==0)
	    				v[1]=0;
	    			else
	    				if(strcmp(p2,"--x")==0)
	    					v[1]=1;
	    				else
	    					if(strcmp(p2,"-w-")==0)
	    						v[1]=2;
	    					else
	    						if(strcmp(p2,"-wx")==0)
	    							v[1]=3;
	    						else
	    							if(strcmp(p2,"r--")==0)
	    								v[1]=4;
	    							else
	    								if(strcmp(p2,"r-x")==0)
	    									v[1]=5;
	    								else
	    									if(strcmp(p2,"rw-")==0)
	    										v[1]=6;
	    									else
	    										if(strcmp(p2,"rwx")==0)
	    											v[1]=7;
	    			if(strcmp(p3,"---")==0)
	    				v[2]=0;
	    			else
	    				if(strcmp(p3,"--x")==0)
	    					v[2]=1;
	    				else
	    					if(strcmp(p3,"-w-")==0)
	    						v[2]=2;
	    					else
	    						if(strcmp(p3,"-wx")==0)
	    							v[2]=3;
	    						else
	    							if(strcmp(p3,"r--")==0)
	    								v[2]=4;
	    							else
	    								if(strcmp(p3,"r-x")==0)
	    									v[2]=5;
	    								else
	    									if(strcmp(p3,"rw-")==0)
	    										v[2]=6;
	    									else
	    										if(strcmp(p3,"rwx")==0)
	    											v[2]=7;
	    										
	    			
	    			
	  	   		// in v am de ex 775 si compar cu permisiunea din v2
	  	   		//printf("%d%d%d ",v[0],v[1],v[2]);
	  	   		
	  	   		int ok=0;
	  	   		for(int i=0;i<3;i++)
	  	   		{
	  	   			if(v[i]!=v2[i])
	  	   				ok=1;
	  	   			
	  	   		}
	  	   		if(ok == 0)
	  	   		{
	    				printf("%s\n",filePath);  
	    			}
			}
		}
	}
	closedir(dir);
}


void listRec(char *path)
{
	DIR *dir=NULL;
	struct dirent *entry=NULL;
	char filePath[512];
	struct stat statBuf;
	
	dir=opendir(path);
	if(dir==NULL)
	{
		perror("ERROR");
		printf("\n");
		printf("invalid directory path");
		return;
	}
	while((entry=readdir(dir)) !=NULL)
	{
		if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0)
		{
			snprintf(filePath,512,"%s/%s",path,entry->d_name);
			if(lstat(filePath,&statBuf)==0)
			{
			        printf("%s\n",filePath);
				if(S_ISDIR(statBuf.st_mode))
				{
					listRec(filePath);
				}
			}
		}
	}
	closedir(dir);
}

/*void listRec1(char *path,char *lm)
{
	DIR *dir=NULL;
	struct dirent *entry=NULL;
	char filePath[512];
	struct stat statBuf;
	
	dir=opendir(path);
	if(dir==NULL)
	{
		perror("ERROR");
		printf("\n");
		printf("invalid directory path");
		return;
	}
	while((entry=readdir(dir)) !=NULL)
	{
		if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0)
		{
			snprintf(filePath,512,"%s/%s",path,entry->d_name);
			if(lstat(filePath,&statBuf)==0)
			{
			        printf("%s\n",filePath);
				if(S_ISDIR(statBuf.st_mode))
				{
					printf("%s\n",filePath);
					 int l=strlen(lm);
		    			 int lp=strlen(filePath);
		    			 int x=lp-l;
		    			 int ok=0;
		    			 if(strcmp((filePath + x) , lm)==0)
		    			 {
		    				 ok = 1;
		    			 }
	    				 if(ok == 1)
	    					listRec(filePath);
				}
			}
		}
	}
	closedir(dir);
}
*/



bool parse(char *path,bool print)   
{
	int fd=-1;
	fd=open(path,O_RDONLY);
	bool ok=false;
	if(fd==-1)
	{
		perror("ERROR");
		return false;
	}
	else
	{
		int header_size=0,version=0;
		char magic[3];
		
		lseek(fd,-4,SEEK_END);
		read(fd,&header_size,2);
		read(fd,magic,2);
		magic[2]='\0';
		if(strcmp(magic,"Z5")==0)
		{
			lseek(fd,-header_size,SEEK_END);
			read(fd,&version,2);
			read(fd,&nr_file_sect,1);
			
			if(section_headers!=NULL)
				free(section_headers);
			section_headers = (Section_Header*)malloc(nr_file_sect*sizeof(Section_Header));
			
			for(int i=0;i<nr_file_sect;i++)
			{
				read(fd,section_headers[i].sect_name,16);
				section_headers[i].sect_name[16]='\0';
				read(fd,&(section_headers[i].sect_type),4);
				read(fd,&(section_headers[i].sect_offset),4);
				read(fd,&(section_headers[i].sect_size),4);
			}
		}
			
		if(strcmp(magic,"Z5")!=0)
		{
			if(print)
			{
				printf("ERROR\n");
				printf("wrong magic\n");
			}
		}
		else
			if(version<13 || version>43)
			{
				if(print)
				{
					printf("ERROR\n");
					printf("wrong version\n");
				}
			}
			else
				if(nr_file_sect<3 || nr_file_sect>11)
				{
					if(print)
					{
						printf("ERROR\n");
						printf("wrong sect_nr\n");
					}
				}
				else
				{
					int ok1=0;
					for(int i=0;i<nr_file_sect;i++) {
						if(section_headers[i].sect_type!=18 && section_headers[i].sect_type!=40 && section_headers[i].sect_type!=48 && section_headers[i].sect_type!=43 && section_headers[i].sect_type!=88 && section_headers[i].sect_type!=14 && section_headers[i].sect_type!=81)
							ok1=1;
					}		
					if(ok1==1)
					{
						if(print)
						{
							printf("ERROR\n");
							printf("wrong sect_types\n");
						}
					}
					else
					{
						if(print){
							printf("SUCCESS\n");
							printf("version=%d\n",version);
							printf("nr_sections=%d\n",nr_file_sect);
							for(int i=0;i<nr_file_sect;i++) {
								printf("section%d: %s %d %d\n",i+1,section_headers[i].sect_name,section_headers[i].sect_type,section_headers[i].sect_size);
							}
						}
						ok=true;
					}	
				}
	
		
	}
	
	close(fd);
	return ok;
}

/*
void parse(char *path)   
{
	int fd=-1;
	fd=open(path,O_RDONLY);
	if(fd==-1)
	{
		perror("ERROR");
		return;
	}
	else
	{
		int header_size=0,version=0,nr_of_sections=0,sect_type[12]={0},sect_size[12]={0},sect_offset[12]={0};
		char magic[3],sect_name[100][17];
		lseek(fd,-4,SEEK_END);
		read(fd,&header_size,2);
		read(fd,magic,2);
		magic[2]='\0';
		lseek(fd,-header_size,SEEK_END);
		read(fd,&version,2);
		read(fd,&nr_of_sections,1);
		for(int i=0;i<nr_of_sections;i++)
		{
			read(fd,sect_name[i],16);
			sect_name[i][16]='\0';
			read(fd,&sect_type[i],4);
			read(fd,&sect_offset[i],4);
			read(fd,&sect_size[i],4);
		}
		if(strcmp(magic,"Z5")!=0)
		{
			printf("ERROR\n");
			printf("wrong magic\n");
		}
		else
			if(version<13 || version>43)
			{
				printf("ERROR\n");
				printf("wrong version\n");
			}
			else
				if(nr_of_sections<3 || nr_of_sections>11)
				{
					printf("ERROR\n");
					printf("wrong sect_nr\n");
				}
				else
				{
					int ok=0;
					for(int i=0;i<nr_of_sections;i++)
						if(sect_type[i]!=18 && sect_type[i]!=40 && sect_type[i]!=48 && sect_type[i]!=43 && sect_type[i]!=88 && sect_type[i]!=14 && sect_type[i]!=81)
							ok=1;
					if(ok==1)
					{
						printf("ERROR\n");
						printf("wrong sect_types\n");
					}
					else
					{
						printf("SUCCESS\n");
						printf("version=%d\n",version);
						printf("nr_sections=%d\n",nr_of_sections);
						for(int i=0;i<nr_of_sections;i++)
							printf("section%d: %s %d %d\n",i+1,sect_name[i],sect_type[i],sect_size[i]);
					}	
				}
			
		
	}
	close(fd);
}*/

void extract(char *path,int section,int line)   
{
	if(parse(path,false))
	{	
		if(section>0 && section<=nr_file_sect)
		{
			int fd=-1;
			fd=open(path,O_RDONLY);
			if(fd==-1)
			{
				perror("ERROR");
				return;
			}
			else
			{
				lseek(fd,section_headers[section-1].sect_offset,SEEK_SET);
				//printf("Dbg offset %X\n", section_headers[section-1].sect_offset);
				int len=section_headers[section-1].sect_size;
				int cnt=0;
				char *v=(char*)malloc(len);
				char *strl_line=v;
				read(fd,v,len);
				bool linie_ok=false;
				for(int i=0;i<len;i++)
				{
					if(v[i]=='\n')
					{
						cnt++;
						if(cnt==line)
						{
							v[i]=0;
							printf("SUCCESS\n");
							printf("%s\n", strl_line);
							linie_ok=true;
							break;
						}
						//printf("Dbg %s\n", strl_line);
						strl_line=&v[i+1];
					}
				}
				if(cnt==line-1 && strl_line<&v[len])
				{
					printf("SUCCESS\n");
					printf("%s\n", strl_line);
					linie_ok=true;
				}	
				if(linie_ok==false)
					printf("ERROR\ninvalid line\n");
					
				
				free(v);
				
			}
		}
		else
		{
			printf("ERROR\ninvalid section\n");
		}
	}
	else
	{
		printf("ERROR\ninvalid file\n");
	}
}

bool findall(char *path)   
{
	//printf("DBG222222 path=%s",path);
	bool ok=false;
	if(parse(path,false))
	{	
	//printf("DBG1111");
		int fd=-1;
		fd=open(path,O_RDONLY);
		if(fd==-1)
		{
			perror("ERROR");
			return false;
		}
		else
		{
			
			//printf("Dbg offset %X\n", section_headers[section-1].sect_offset);
			for(int j=0;j<nr_file_sect;j++)
			{
				//printf("DBGG");
				lseek(fd,section_headers[j-1].sect_offset,SEEK_SET);
				int len=section_headers[j-1].sect_size;
				int cnt=0;
				char *v=(char*)malloc(len);
				char *strl_line=v;
				read(fd,v,len);
				
				for(int i=0;i<len;i++)
				{
					if(v[i]=='\n')
					{
						cnt++;
						strl_line=&v[i+1];
					}
				}
				
				if(strl_line<&v[len])
				{
					cnt++;
				}
				//printf("%s sectiune:%d cnt:%d",path,j,cnt);	
				free(v);

				if(cnt>=16)
					ok=true;
			}
			
		}
	}
		
	return ok;
}

void sectiuni(char *path)
{
	DIR *dir=NULL;
	struct dirent *entry=NULL;
	char filePath[512];
	struct stat statBuf;
	
	dir=opendir(path);
	if(dir==NULL)
	{
		perror("ERROR");
		printf("\n");
		printf("invalid directory path");
		return;
	}
	while((entry=readdir(dir)) !=NULL)
	{
		if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0)
		{
			snprintf(filePath,512,"%s/%s",path,entry->d_name);
			if(lstat(filePath,&statBuf)==0)
			{	
				
			       
				if(S_ISDIR(statBuf.st_mode))
				{
					sectiuni(filePath);
					
				}
				else
				{
					if(findall(filePath))
						{
						printf("%s\n",filePath);	
						
						}
				}
			}
		}
	}
	closedir(dir);
}


int main(int argc, char **argv){
   char path[512],cf[512],perm[15],section[3],line[3];
    struct stat statBuf;
    if(argc >= 2){
       if(strcmp(argv[1], "variant") == 0){
            printf("92790\n");
        }
        else
        {
        	if(strcmp(argv[1],"list")==0) 
        	{
        		int recok=0,cfok=0,permok=0;
        		for(int i=2;i<argc;i++)
	    		{
	    			if(strstr(argv[i],"recursive")!=NULL){
	    				recok=1;
	    			}	
	    			if(strstr(argv[i],"path=")!=NULL)
	    			{
	    				strcpy(path,argv[i]+5);
	    			}
	    			
	    			if(strstr(argv[i],"name_ends_with=")!=NULL)
	    			
	    			{
	    				cfok=1;
	    				strcpy(cf,argv[i]+15);
	    			}
	    			//if(strstr(argv[i],"permissions=")!=NULL)
	    			if(strncmp(argv[i],"permissions=",12)==0)
	    			{
	    				permok=1;
	    				strcpy(perm,argv[i]+12);
	    				
	    			}
	    			
	    		}
	    		if(lstat(path,&statBuf)==0)
	    		{
	  			if(recok==0 && cfok==0 && permok==0)
	   			{
		    			list(path);
	    			}
	    			else
	    				if(recok==1)
	    				{	
	    					printf("SUCCESS");
	    					printf("\n");
			    			listRec(path);
	    				}
	    			if(cfok==1)
	    			{
	    				if(recok==0)
	    					list1(path,cf);
	    				/*else
	    					if(recok==1)
	    						listRec1(path,cf);*/
	    					
	    			}
	    			
	    			if(permok==1)
	    			
	    			{
	    				if(recok==0)
	    				{
	    				//printf("%s ",perm);
	    					list_permisiuni(path,perm);
	    				}
	    			}
	    			
	    		}
	    	}
	    	if(strcmp(argv[1],"parse")==0) 
	    	{
	    		if(strstr(argv[2],"path=")!=NULL)
	    		{
	    			strcpy(path,argv[2]+5);
	    		}
	    		if(lstat(path,&statBuf)==0)
	    		{
	    		//printf("%s",path);
	    			parse(path,true);
	    		}
	    		
	    	}
	    	if(strcmp(argv[1],"extract")==0) 
	    	{
	    		int nr_section,nr_line;
	    		if(strstr(argv[2],"path=")!=NULL)
	    		{
	    			strcpy(path,argv[2]+5);
	    		}
	    		if(strstr(argv[3],"section=")!=NULL)
	    		{
	    			strcpy(section,argv[3]+8);
	    			 nr_section=atoi(section);
	    			//printf("%d ",nr_section);
	    		}
	    		if(strstr(argv[4],"line=")!=NULL)
	    		{
	    			strcpy(line,argv[4]+5);
	    			nr_line=atoi(line);
	    			//printf("%d",nr_line);
	    		}
	    		if(lstat(path,&statBuf)==0)
	    		{
	    		//printf("%s",path);
	    			extract(path,nr_section,nr_line);
	    		}	
	    	}
	    	if(strcmp(argv[1],"findall")==0) 
	    	{
	    		if(strstr(argv[2],"path=")!=NULL)
	    		{
	    			strcpy(path,argv[2]+5);
	    		}
	    		if(lstat(path,&statBuf)==0)
	    		{
	    		//printf("%s",path);
	    			sectiuni(path);
	    		}	
	    	}
	    }
	 }
	if(section_headers!=NULL)
		free(section_headers);
	 return 0;
 }
	    				
