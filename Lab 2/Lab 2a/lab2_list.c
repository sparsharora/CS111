/*                                                                                                                                                                                                          
Sparsh Arora                                                                                                                                                                                                
UID: 804653078                                                                                                                                                                                              
sparsharora@g.ucla.edu                                                                                                                                                                                      
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "SortedList.h"
#include <string.h>

int it=1, th=1, size=0;
int opt_yield=0;
int iflag=0, yflag=0, dflag=0, lflag=0;
int sync_f=0,mflag=0, sflag=0;
SortedList_t *list;
SortedListElement_t *nodes;
pthread_mutex_t lock;
int s_lock=0;

void signal_handler(int sig)
{
  if(sig==SIGSEGV)
    {
      fprintf(stderr,"Segmentation Fault detected!! \n");
      exit(2);
    }
}


void* list_op(void* id)
{
  int k, rc;
  //fprintf(stderr,"here10\n");
  int pid=*(int*)id;

  //fprintf(stderr,"%d\n",pid);
  for(k=pid;k<(th*it);k+=th)
    {
      if(sync_f==1)
	{
	  if(mflag==1)
	    {
	      pthread_mutex_lock(&lock);
	      SortedList_insert(list,&nodes[k]);
	      pthread_mutex_unlock(&lock);
	    }
	  if(sflag==1)
	    {
	      while (__sync_lock_test_and_set(&s_lock, 1));
	      SortedList_insert(list,&nodes[k]);
	      __sync_lock_release(&s_lock);
	    }
	}
      else
	{
	  SortedList_insert(list,&nodes[k]);
	  //fprintf(stderr,"HERE\n");
	}
    }
  
  //  fprintf(stderr,"middle\n");
  for(k=pid;k<(th*it);k+=th)
    {
    SortedListElement_t *store;
    
    if(sync_f==1)
    {
    if(mflag==1)
	    {
	      pthread_mutex_lock(&lock);
	      store=SortedList_lookup(list,(nodes+k)->key);
	      if(store==NULL)
		{
		  fprintf(stderr,"Unable to find element! \n");
		  exit(2);
		}
	      else
		{
		  rc=SortedList_delete(store);
		  if(rc)
		    {
		      fprintf(stderr,"Unable to delete element! \n");
		      exit(2);
		    }
		}
	      pthread_mutex_unlock(&lock);
	    }
	  if(sflag==1)
	    {
	      while (__sync_lock_test_and_set(&s_lock, 1));
	      store=SortedList_lookup(list,(nodes+k)->key);
              if(store==NULL)
                {
                  fprintf(stderr,"Unable to find element! \n");
                  exit(2);
                }
              else
                {
                  rc=SortedList_delete(store);
                  if(rc)
                    {
                      fprintf(stderr,"Unable to delete element! \n");
                      exit(2);
                    }
                }
	      __sync_lock_release(&s_lock);
	    }
	}
      else
	{
	  store=SortedList_lookup(list,(nodes+k)->key);
	  if(store==NULL)
	    {
	      fprintf(stderr,"Unable to find element! \n");
	      exit(2);
	    }
	  else
	    {
	      rc=SortedList_delete(store);
	      if(rc)
		{
		  fprintf(stderr,"Unable to delete element! \n");
		  exit(2);
		}
	    }
	}
	      
    }
  // fprintf(stderr,"here3\n");
  size=SortedList_length(list);
  // return NULL;
}
      
int
main(int argc, char **argv)
{

  int i=0;
  struct timespec start_time;
  struct timespec end_time;
  long long ns;

  char c;
  char* check;

  signal(SIGSEGV, signal_handler);
  
  while (1) {
    static struct option long_options[] = {
      {"threads", 1, 0, 't'},
      {"iterations", 1, 0, 'i'},
      {"yield", 1, 0, 'y'},
      {"sync", 1, 0, 's'},
      {0, 0, 0, 0}
    };

    c = getopt_long(argc, argv, "t:i:y:s:",
                    long_options, NULL );
    if(c == -1)
      break;

    switch (c) {
    case 't':
      th=atoi(optarg);
      break;
    case 'i':
      it=atoi(optarg);
      break;
    case 'y':
      yflag=1;
      check=optarg;
      while(check[i]!='\0')
	{
	  if(check[i]=='i')
	    {
	      opt_yield|=INSERT_YIELD;
	      iflag=1;
	    }
	  else if(check[i]=='d')
            {
	      opt_yield|=DELETE_YIELD;
	      dflag=1;
            }
	  else if(check[i]=='l')
            {
	      opt_yield|=LOOKUP_YIELD;
	      lflag=1;
            }
	  else
	    {
	      fprintf(stderr,"Correct usage: ./lab2_list [--threads=] [--iterations=] [--sync=] [--yield=[idl]] ... \n");
	      exit(1);
	    }
	  i++;
	}
      break;
    case 's':
      sync_f=1;
      if(optarg[0]=='m')
	mflag=1;
      else if (optarg[0]=='s')
	sflag=1;
      else
	{
	  fprintf(stderr,"Correct usage: ./lab2_list [--threads=] [--iterations=] [--sync=] [--yield=[idl]] ... \n");
	  exit(1);
	}
      break;
    default:
      {
	fprintf(stderr,"Correct usage: ./lab2_list [--threads=] [--iterations=] [--sync=] [--yield=[idl]] ... \n");
	exit(1);
      }
    }
  }

  // fprintf(stderr,"here\n");
  list= (SortedList_t *) malloc(sizeof(SortedList_t));
  list->next=NULL;
  list->prev=NULL;
  list->key=NULL;

  nodes=(SortedListElement_t*) malloc(th*it*sizeof(SortedListElement_t));
  srand((unsigned) time(NULL));

  char key_set[]="0123456789abcdefghijklmnopqrstuvwxyz";
  int ksize;

  for(i=0;i<(it*th);i++)
    {

      ksize=rand()%12+3;
      char *key=(char *) malloc((ksize+1)*sizeof(char));
      int k=0;
      for(;k<ksize;k++)
	{
	  key[k] = key_set[rand()%strlen(key_set)];
	}
      key[k]='\0';

      nodes[i].key=key;
    }

  //fprintf(stderr,"here2\n");
  clock_gettime(CLOCK_MONOTONIC, &start_time);


  
  pthread_t *thread=(pthread_t*)malloc(th*sizeof(pthread_t));
  int *p_id=(int*)malloc(th*sizeof(int));
  // fprintf(stderr,"here\n");
  int rc;
  long t;
  void *status;
  //fprintf(stderr,"here\n");
  for(t=0; t<th; t++)
    {
      p_id[t]=t;
      rc = pthread_create(thread+t, NULL, list_op, &p_id[t]);
      if(rc)
        {
          fprintf(stderr, "ERROR!, Unable to create thread! \n");
          exit(1);
        }
      // fprintf(stderr,"there\n");
    }

  for(t=0; t<th; t++)
    {
      rc = pthread_join(*(thread+t), &status);
      if(rc)
        {
          fprintf(stderr, "ERROR!, Unable to join thread! \n");
          exit(1);
        }
    }

  clock_gettime(CLOCK_MONOTONIC, &end_time);
  ns = end_time.tv_sec - start_time.tv_sec;
  ns *= 1000000000;
  ns += end_time.tv_nsec;
  ns -= start_time.tv_nsec;

  
  if(size!=0)
    {
      fprintf(stderr,"ERROR!, Length is not 0! \n");
      exit(2);
    }

  
  char* first="list-";
  char* second;
  char* last;

  if(yflag==0)
    second="none";
  else
    {
      
      if(iflag==1 && dflag==1 && lflag==1)
	second="idl";
      else
	{
	  if(iflag==1)
	    {
	      if(dflag==1 && lflag==0)
		second="id";
	      else if(dflag==0 && lflag==1)
		second="il";
	      else
		second="i";
	    }
	  else if(dflag==1)
	    {
	      if(lflag==1)
		second="dl";
	      else
		second="d";
	    }
	  else
	    second="l";
	}
      
	 
    }

  if(sync_f==0)
    last="-none";
  else
    {
      if(mflag==1)
	last="-m";
      if(sflag==1)
	last="-s";
    }

  fprintf(stdout, "%s%s%s,%d,%d,1,%d,%d,%d\n", first,second,last,th,it,(th*it*3),ns,ns/(th*it*3));
  return 0;
}
