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
pthread_mutex_t *lock;
int *s_lock;
int test_lists=1;
long long mutex_ns=0;

void signal_handler(int sig)
{
  if(sig==SIGSEGV)
    {
      fprintf(stderr,"Segmentation Fault detected!! \n");
      exit(2);
    }
}

unsigned long
hash(const char *str)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

void* list_op(void* id)
{
  struct timespec start_time;
  struct timespec end_time;
  long long ns=0;
  int k, rc;
  //fprintf(stderr,"here10\n");
  int pid=*(int*)id;
  unsigned long list_num;
  //fprintf(stderr,"%d\n",pid);
  for(k=pid;k<(th*it);k+=th)
    {
      list_num=(hash((nodes+k)->key))%test_lists;
      if(sync_f==1)
	{
	  if(mflag==1)
	    {
	      clock_gettime(CLOCK_MONOTONIC, &start_time);
	      pthread_mutex_lock(lock+list_num);
	      clock_gettime(CLOCK_MONOTONIC, &end_time);
	      
	      ns = end_time.tv_sec - start_time.tv_sec;
	      ns *= 1000000000;
	      ns += end_time.tv_nsec;
	      ns -= start_time.tv_nsec;
	      mutex_ns+=ns;
	      
	      SortedList_insert(list+list_num,&nodes[k]);
	      pthread_mutex_unlock(lock+list_num);
	    }
	  if(sflag==1)
	    {
	      clock_gettime(CLOCK_MONOTONIC, &start_time);
	      while (__sync_lock_test_and_set(s_lock+list_num, 1));
	      clock_gettime(CLOCK_MONOTONIC, &end_time);

              ns = end_time.tv_sec - start_time.tv_sec;
              ns *= 1000000000;
              ns += end_time.tv_nsec;
              ns -= start_time.tv_nsec;
              mutex_ns+=ns;

	      SortedList_insert(list+list_num,&nodes[k]);
	      __sync_lock_release(s_lock+list_num);
	    }
	}
      else
	{
	  SortedList_insert(list+list_num,&nodes[k]);
	  //fprintf(stderr,"HERE\n");
	}
    }
  
  for(k=pid;k<(th*it);k+=th)
    {
      list_num=(hash((nodes+k)->key))%test_lists;
      if(sync_f==1)
        {
          if(mflag==1)
            {
	      clock_gettime(CLOCK_MONOTONIC, &start_time);
              pthread_mutex_lock(lock+list_num);
              clock_gettime(CLOCK_MONOTONIC, &end_time);

              ns = end_time.tv_sec - start_time.tv_sec;
              ns *= 1000000000;
              ns += end_time.tv_nsec;
              ns -= start_time.tv_nsec;
              mutex_ns+=ns;

	      size=SortedList_length(list+list_num);
	      if(size==-1)
		{
		  fprintf(stderr,"CORRUPTED list length!!\n");
		  exit(2);
		}
              pthread_mutex_unlock(lock+list_num);
            }
          if(sflag==1)
            {
	      clock_gettime(CLOCK_MONOTONIC, &start_time);
              while (__sync_lock_test_and_set(s_lock+list_num, 1));
	      clock_gettime(CLOCK_MONOTONIC, &end_time);

              ns = end_time.tv_sec - start_time.tv_sec;
              ns *= 1000000000;
              ns += end_time.tv_nsec;
              ns -= start_time.tv_nsec;
              mutex_ns+=ns;
	      
	      size=SortedList_length(list+list_num);
	      if(size==-1)
		{
                  fprintf(stderr,"CORRUPTED list length!!\n");
		  exit(2);
                } 
	      __sync_lock_release(s_lock+list_num);
            }
        }
      else
        {
	  size= SortedList_length(list+list_num);
	  if(size==-1)
	    {
	      fprintf(stderr,"CORRUPTED list length!!\n");
	      exit(2);
	    }
                                                                                                                                                                       
        }
    }

  //  fprintf(stderr,"middle\n");
  for(k=pid;k<(th*it);k+=th)
    {
      SortedListElement_t *store;
      list_num=(hash((nodes+k)->key))%test_lists;
      if(sync_f==1)
	{
	  if(mflag==1)
	    {
	      clock_gettime(CLOCK_MONOTONIC, &start_time);
	      pthread_mutex_lock(lock+list_num);
	      clock_gettime(CLOCK_MONOTONIC, &end_time);

              ns = end_time.tv_sec - start_time.tv_sec;
              ns *= 1000000000;
              ns += end_time.tv_nsec;
              ns -= start_time.tv_nsec;
              mutex_ns+=ns;

	      store=SortedList_lookup(list+list_num,(nodes+k)->key);
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
	      pthread_mutex_unlock(lock+list_num);
	    }
	  if(sflag==1)
	    {
	      clock_gettime(CLOCK_MONOTONIC, &start_time);
	      while (__sync_lock_test_and_set(s_lock+list_num, 1));
	      clock_gettime(CLOCK_MONOTONIC, &end_time);

              ns = end_time.tv_sec - start_time.tv_sec;
              ns *= 1000000000;
              ns += end_time.tv_nsec;
              ns -= start_time.tv_nsec;
              mutex_ns+=ns;

	      store=SortedList_lookup(list+list_num,(nodes+k)->key);
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
	      __sync_lock_release(s_lock+list_num);
	    }
	}
      else
	{
	  store=SortedList_lookup(list+list_num,(nodes+k)->key);
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
  for(k=pid;k<(th*it);k+=th)
    {
      list_num=(hash((nodes+k)->key))%test_lists;
      if(sync_f==1)
        {
          if(mflag==1)
            {
              pthread_mutex_lock(lock+list_num);
              size=SortedList_length(list+list_num);
              if(size==-1)
                {
                  fprintf(stderr,"CORRUPTED list length!!\n");
                  exit(2);
                }
              pthread_mutex_unlock(lock+list_num);
            }
          if(sflag==1)
            {
              while (__sync_lock_test_and_set(s_lock+list_num, 1));
              size=SortedList_length(list+list_num);
              if(size==-1)
                {
                  fprintf(stderr,"CORRUPTED list length!!\n");
                  exit(2);
                }
              __sync_lock_release(s_lock+list_num);
            }
        }
      else
        {
          size= SortedList_length(list+list_num);
          if(size==-1)
            {
              fprintf(stderr,"CORRUPTED list length!!\n");
	      exit(2);
            }

        }
    }
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
      {"lists", 1, 0, 'l'},
      {0, 0, 0, 0}
    };

    c = getopt_long(argc, argv, "t:i:y:s:l:",
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
    case 'l':
      test_lists=atoi(optarg);
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
  list = (SortedList_t *) malloc(test_lists*sizeof(SortedList_t));
  /*list->next=NULL;
  list->prev=NULL;
  list->key=NULL;
  */

  for(i=0;i<test_lists;i++)
    {
      SortedList_t *temp=&list[i];
      temp->next=NULL;
      temp->prev=NULL;
      temp->key=NULL;
    }

  if(sync_f && mflag)
    {
      lock=(pthread_mutex_t*)malloc(test_lists*sizeof(pthread_mutex_t));
      for(i=0;i<test_lists;i++)
	pthread_mutex_init(&lock[i],NULL);
    }
  else if(sync_f && sflag)
    {
      s_lock=(int *)malloc(test_lists*sizeof(int));
      for(i=0;i<test_lists;i++)
	{
	  s_lock[i]=0;
	}
    }
      

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

  fprintf(stdout, "%s%s%s,%d,%d,%d,%d,%d,%d,%d\n", first,second,last,th,it,test_lists,(th*it*3),ns,ns/(th*it*3),mutex_ns/(th*it*3));
  return 0;
}
