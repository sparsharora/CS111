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
#include <pthread.h>

int it=1;
int th=1;
pthread_mutex_t add_mutex;
int s_lock=0;
int opt_yield=0;
int mflag=0, cflag=0, sflag=0;

void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
}



void cas_helper(long long *ptr, long long val)
{
  long long old, new;
  do
    {
      old = *ptr;
      new = old + val;
      if(opt_yield)
	sched_yield();
    } while( __sync_val_compare_and_swap(ptr, old, new) != old);
}


void* add_helper(void *ptr)
{
  int i;
  for(i=0; i<it ; i++)
    {
      if(mflag==1)
	{
	  pthread_mutex_lock(&add_mutex);
	  add((long long *)ptr, 1);
	  add((long long *)ptr,-1);
	  pthread_mutex_unlock(&add_mutex);
	}

       else if(sflag==1)
	{
	  while (__sync_lock_test_and_set(&s_lock, 1));
	  add((long long *)ptr, 1);
          add((long long *)ptr,-1);
	  __sync_lock_release(&s_lock);
	}

      else if(cflag==1)
	{
	  cas_helper((long long*)ptr,1); 
	  cas_helper((long long*)ptr,-1);
	  }
      else
	{
	  add((long long *)ptr, 1);
          add((long long *)ptr,-1);
	}
	  
    }

 
}
      


int
main(int argc, char **argv)
{

 
  struct timespec start_time;
  struct timespec end_time;
  long long ns;

  char c; 
  char* check;
  //int mflag=0, cflag=0, sflag=0;
  long long ctr=0;
  //int th=1, it=1;
  while (1) {
    static struct option long_options[] = {
      {"threads", 1, 0, 't'},
      {"iterations", 1, 0, 'i'},
      {"yield", 0, 0, 'y'},
      {"sync", 1, 0, 's'},
      {0, 0, 0, 0}
    };
    
    c = getopt_long(argc, argv, "t:i:y",
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
      opt_yield=1;
      break;
    case 's':
      check=optarg;
      // fprintf(stderr,check);
      if(check[0]=='m')
	mflag=1;
      else if(check[0]=='s')
	sflag=1;
      else if(check[0]=='c')
	cflag=1;
      else
	{
	  fprintf(stderr,"Correct usage: lab2_add [--threads=] [--iterations=] [--sync=] [--yield] ... \n");
	  exit(1);
	}
      break;
    default:
      {
	fprintf(stderr,"Correct usage: lab2_add [--threads=] [--iterations=] [--sync=] [--yield] ... \n");
	exit(1);
      }
    }
  }

  
  clock_gettime(CLOCK_MONOTONIC, &start_time);


  pthread_t thread[th];

  int rc;
  long t;
  void *status;

  for(t=0; t<th; t++)
    {
      rc = pthread_create(&thread[t], NULL, add_helper, (void *)&ctr); 
      if(rc)
	{
	  fprintf(stderr, "ERROR!, Unable to create thread! \n");
	  exit(1);
	}
    }

  for(t=0; t<th; t++)
    {
      rc = pthread_join(thread[t], &status);
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

  if(opt_yield==1 && mflag==1)
    {
      fprintf(stdout, "add-yield-m,");
    }
  else if(opt_yield==1 && cflag==1)
    {
      fprintf(stdout, "add-yield-c,");
    }
  else if(opt_yield==1 && sflag==1)
    {
      fprintf(stdout, "add-yield-s,");
    }
  else if(opt_yield==1)
    {
      fprintf(stdout, "add-yield-none,");
    }
  else if(cflag==1)
    {
      fprintf(stdout, "add-c,");
    }
  else if(mflag==1)
    {
      fprintf(stdout, "add-m,");
    }
  else if(sflag==1)
    {
      fprintf(stdout, "add-s,");
    }
  else
    {
      fprintf(stdout, "add-none,");
    }

  fprintf(stdout, "%d,%d,%d,%d,%d,", th, it, (th*it*2), ns, ns/(th*it*2));
  fprintf(stdout, "%lld\n", ctr);
  exit(0);
}
