#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#define NR_THREADS 4
#define NR_THREADS8 5
#define NR_THREADS2 42

typedef struct{
	int nrp; //nr proces
	int nrt; //nr thread
}TH_STRUCT;

void *threadFn60(void *unused)
{
	TH_STRUCT *s=(TH_STRUCT*)unused;
	
	info(BEGIN,s->nrp,s->nrt);
	
	sem_t *semafor;
	semafor=sem_open("/my_semaphore",O_CREAT,0644,0);
	
	sem_t *semafor1;
        semafor1=sem_open("/my_semaphore1",O_CREAT,0644,0);
        
	
	for(int i=1;i<=NR_THREADS;i++)
	{
		sem_post(semafor);
	}
	
	for(int i=1;i<=NR_THREADS;i++)
	{
		sem_wait(semafor1);
	}
	info(END,s->nrp,s->nrt);
	
	
       return NULL;
        
}

void *threadFn6x(void *unused)
{
	TH_STRUCT *s=(TH_STRUCT*)unused;
	
	sem_t *semafor;
        semafor=sem_open("/my_semaphore",O_CREAT,0644,0);
        
        sem_t *semafor1;
        semafor1=sem_open("/my_semaphore1",O_CREAT,0644,0);
        
       
        
        sem_wait(semafor);
        
        if(s->nrt!=2 && s->nrt!=3)
	{
		info(BEGIN,s->nrp,s->nrt);
		info(END,s->nrp,s->nrt);
	}
	if(s->nrt==3)
	{
		sem_t *semafor2;
      	        semafor2=sem_open("/my_semaphore2",O_CREAT,0644,0);
        
    	        sem_t *semafor3;
                semafor3=sem_open("/my_semaphore3",O_CREAT,0644,0);
        
		sem_wait(semafor3);
		
		info(BEGIN,s->nrp,3);
		info(END,s->nrp,3);
		sem_post(semafor2);
		
	}
	if(s->nrt==2)
	{
		sem_t *semafor2;
      	        semafor2=sem_open("/my_semaphore2",O_CREAT,0644,0);
        
    	        sem_t *semafor3;
                semafor3=sem_open("/my_semaphore3",O_CREAT,0644,0);
                
                
		info(BEGIN,s->nrp,2);
		sem_post(semafor3);
		sem_wait(semafor2);
		info(END,s->nrp,2);
	}
	
	sem_post(semafor1);
	return NULL;
}

/*void *threadFn420(void *unused)
{
	TH_STRUCT *s=(TH_STRUCT*)unused;
	
	info(BEGIN,s->nrp,s->nrt);
	
	sem_t *semafor4;
	semafor4=sem_open("/my_semaphore4",O_CREAT,0644,0);
	
	sem_t *semafor5;
        semafor5=sem_open("/my_semaphore5",O_CREAT,0644,0);
        
	
	for(int i=1;i<=NR_THREADS2;i++)
	{
		sem_post(semafor4);
	}
	
	for(int i=1;i<=NR_THREADS2;i++)
	{
		sem_wait(semafor5);
	}
	info(END,s->nrp,s->nrt);
	
	
       return NULL;
        
}*/

/*void *threadFn42x(void *unused)
{
	TH_STRUCT *s=(TH_STRUCT*)unused;
	
	sem_t *semafor4;
	semafor4=sem_open("/my_semaphore4",O_CREAT,0644,0);
	
	sem_t *semafor5;
        semafor5=sem_open("/my_semaphore5",O_CREAT,0644,0);
        
       info(BEGIN,s->nrp,s->nrt);
        
        sem_wait(semafor4);*/
        
      /*  if(s->nrt!=2 && s->nrt!=3)
	{
		info(BEGIN,s->nrp,s->nrt);
		info(END,s->nrp,s->nrt);
	}
	if(s->nrt==3)
	{
		sem_t *semafor2;
      	        semafor2=sem_open("/my_semaphore2",O_CREAT,0644,0);
        
    	        sem_t *semafor3;
                semafor3=sem_open("/my_semaphore3",O_CREAT,0644,0);
        
		sem_wait(semafor3);
		
		info(BEGIN,s->nrp,3);
		info(END,s->nrp,3);
		sem_post(semafor2);
		
	}
	if(s->nrt==2)
	{
		sem_t *semafor2;
      	        semafor2=sem_open("/my_semaphore2",O_CREAT,0644,0);
        
    	        sem_t *semafor3;
                semafor3=sem_open("/my_semaphore3",O_CREAT,0644,0);
                
                
		info(BEGIN,s->nrp,2);
		sem_post(semafor3);
		sem_wait(semafor2);
		info(END,s->nrp,2);
	}*/
	/*info(END,s->nrp,s->nrt);
	sem_post(semafor5);
	
	return NULL;
}*/

void *threadFn(void *unused)
{
	TH_STRUCT *s=(TH_STRUCT*)unused;
	info(BEGIN,s->nrp,s->nrt);
	info(END,s->nrp,s->nrt);
	
       // semafor=sem_open("/my_semaphore",O_CREAT,0644,1);
      //  semafor1=sem_open("/my_semaphore1",O_CREAT,0644,1);
       /* if(s->nrp==6 && s->nrt!=2)
        {
        	sem_wait(semafor);
        	
		info(BEGIN,s->nrp,s->nrt);
		info(END,s->nrp,s->nrt);
		
		sem_post(semafor);
	}
	else 
	{
		info(BEGIN,s->nrp,s->nrt);
		info(END,s->nrp,s->nrt);
	}*/
	
	/*if(s->nrp==6 && s->nrt!=2 && s->nrt!=3)
	{
		info(BEGIN,s->nrp,s->nrt);
		info(END,s->nrp,s->nrt);
	}
	if(s->nrp==6 && s->nrt==3)
	{
		sem_wait(semafor1);
		
		info(BEGIN,s->nrp,2);
		info(END,s->nrp,2);
		
		//sem_post(semafor1);
		
	}
	if(s->nrp==6 && s->nrt==2)
	{
		//sem_wait(semafor1);
		
		info(BEGIN,s->nrp,3);
		sem_post(semafor1);
		info(END,s->nrp,3);
		
		
		
	}*/
	
	return NULL;
		
}

int main(){
    pid_t p2=-1,p3=-1,p4=-1,p5=-1,p6=-1,p7=-1,p8=-1,p9=-1;
    pthread_t tid[NR_THREADS+1];
    TH_STRUCT params[NR_THREADS+1];
    
    pthread_t tid8[NR_THREADS8];
    TH_STRUCT params8[NR_THREADS8];
    
    pthread_t tid2[NR_THREADS2];
    TH_STRUCT params2[NR_THREADS2];
    
   // pthread_t tid2[NR_THREADS2+1];
    //TH_STRUCT params2[NR_THREADS2+1];
    
    init();

    info(BEGIN, 1, 0);
    
    p2=fork();
   
    if(p2==0)
    {
       info(BEGIN,2,0);
        
    	p3=fork();
    	if(p3==0)
    	{
	    	info(BEGIN,3,0);
	    	info(END, 3, 0);
	    	exit(0);
    	}
    	else
    	{	//aici lucreaza p2 parintele pt p3          
    		 waitpid(p3,NULL,0);
    		 
    		p4=fork();		
    	
	    	if(p4==0)
	    	{
	    		info(BEGIN,4,0);
	    		p9=fork();
	    		if(p9==0)
	    		{
		    		info(BEGIN,9,0);
		    		info(END, 9, 0);
		    		exit(0);
		    	}
	    		else
	    		{ //lucreaza p4 parintele lui p9
	    			waitpid(p9,NULL,0);
	    			info(END, 4, 0);
	    			exit(0);
	    		}
	    	}
	    	else
	    	{ //lucreaza p2 parinte pt p4
	    		waitpid(p4,NULL,0);
	    		
	    	}
	    	
	    	p5=fork();
	    	if(p5==0)
	    	{
		    	info(BEGIN,5,0);
		    	info(END, 5, 0);
		    	exit(0);
	    	}
	    	else
	    	{
	    		waitpid(p5,NULL,0);
	    	}
	    	
	    	p7=fork();	
	    		
	    	if(p7==0)
	    	{
	    		info(BEGIN,7,0);
	    		p8=fork();
	    		if(p8==0)
	    		{
		    		info(BEGIN,8,0);
		    		
		    		for(int i=0;i<NR_THREADS8;i++)
			        {
				    params8[i].nrp=8;
				    params8[i].nrt=i+1;
				    pthread_create(&tid8[i],0,threadFn,&params8[i]);
			        }
			        for(int i=0;i<NR_THREADS8;i++)
			    	        pthread_join(tid8[i],0);
	    	
		    		info(END, 8, 0);
		    		exit(0);
		    	}
		    	else
		    	{ //aici lucreaza p7 parinte pt p8
		    		 waitpid(p8,NULL,0);
		    		 info(END, 7, 0);
	    			 exit(0);
		    	}
	    	}
	    	else
	    	{
	    		waitpid(p7,NULL,0);
	    		
	    	}
	    	
	  /*  params2[0].nrp=2;
	    params2[0].nrt=0;
	    pthread_create(&tid2[0],0,threadFn420,&params2[0]);
	    for(int i=1;i<=NR_THREADS2;i++)
	    {
	    	params[i].nrp=2;
	    	params[i].nrt=i;
	    	pthread_create(&tid2[i],0,threadFn42x,&params2[i]);
	    }*/
	    for(int i=0;i<NR_THREADS2;i++)
		{
			  params2[i].nrp=2;
			  params2[i].nrt=i+1;
			  pthread_create(&tid2[i],0,threadFn,&params2[i]);
		 }	
	         for(int i=0;i<NR_THREADS2;i++)
		          pthread_join(tid2[i],0);

	    	info(END,2,0);
    		 exit(0);
	    }
	    
    }
    else
    {
    	  waitpid(p2,NULL,0);
    }
    
    p6=fork();
    if(p6==0)
    {
	   // info(BEGIN,6,0); 
	    
	    params[0].nrp=6;
	    params[0].nrt=0;
	   pthread_create(&tid[0],0,threadFn60,&params[0]);
	    for(int i=1;i<=NR_THREADS;i++)
	    {
	    	params[i].nrp=6;
	    	params[i].nrt=i;
	    	pthread_create(&tid[i],0,threadFn6x,&params[i]);
	    }
	    for(int i=0;i<=NR_THREADS;i++)
	    	pthread_join(tid[i],0);
	    
	   // info(END,6,0); 
	    exit(0);
    }
    else
    {
  	  waitpid(p6,NULL,0);
    }
    
    info(END, 1, 0);
    
    return 0;
}

