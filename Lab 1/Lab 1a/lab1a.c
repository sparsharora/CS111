#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

struct termios restore;
int stat;


void reset()
{
  tcsetattr (STDIN_FILENO, TCSANOW, &restore);
}


int
set_istrip (int desc)
{



  struct termios settings;
  int result;

  result = tcgetattr (desc, &settings);
  if (result < 0)
    {
      fprintf(stderr,"Unable to get terminal attributes \n");
      return 0;
    }

  tcgetattr (desc, &restore);
  

  settings.c_iflag = ISTRIP; /* only lower 7 bits*/
  settings.c_oflag = 0; /* no processing*/
  settings.c_lflag = 0; /* no processing*/

  result = tcsetattr (desc, TCSANOW, &settings);
  atexit(reset);
  if (result < 0)
    {
      fprintf(stderr,"Unable to set terminal attributes \n");
      return 0;
    }
  return 1;
}


void signal_handler(int signum)
{
  if(signum==SIGPIPE)
    {
      printf("SHELL EXIT SIGNAL=%d STATUS=%d \n\r", WEXITSTATUS(stat), WTERMSIG(stat));
      exit(0);
    }

}



int
main(int argc, char **argv)
{

  int c;
  int sflag=0; 

 
  while (1) {
    static struct option long_options[] = {
      {"shell", 0, 0, 's'},
      {0, 0, 0, 0}
    };
    
    c = getopt_long(argc, argv, "",
		    long_options, NULL );
    if(c == -1)
      break;
        
    switch (c) {
    case 's':
      sflag=1;
      signal(SIGPIPE, signal_handler);
      break;
    default:
      {
	fprintf(stderr,"Correct usage: lab1a [--shell] ... \n");
	exit(1);
      }
    }
  }

  
  if(!set_istrip(STDIN_FILENO))
    {
      perror ("error in tcgetattr");
      return 1;
    }
  

  


  if(sflag==1)

    {


      int pipefd1[2];    //input passed to shell
      int pipefd2[2];    //output out of shell
      pid_t cpid;
      
      
      
      if (pipe(pipefd1) == -1) {
	fprintf(stderr,"Unable to execute pipe! \n");
	exit(1);
      }
      
      if (pipe(pipefd2) == -1) {
	fprintf(stderr,"Unable to execute pipe! \n");
	exit(1);
      }
      
      cpid = fork();
      if (cpid<0) {
	fprintf(stderr,"Unable to execute fork()! \n");
	exit(1);
      }
      
      else if (cpid == 0) {    /* Child reads from pipe */
	close(pipefd1[1]);          /* Close unused write end */
	close(pipefd2[0]);
	// pipefd1 Child-Read, Terminal-Write
	// pipefd2 Child-Write, Terminal-Read
	dup2(pipefd1[0],0);
	dup2(pipefd2[1],1);
	dup2(pipefd2[1],2);
	
	
	
	char* myargs[2];
	
	myargs[0]=strdup("/bin/bash");
	myargs[1]=NULL;
	if(execvp(myargs[0], myargs)==-1)
	  {
	    fprintf(stderr,"Unable to execute Shell! \n");
	    exit(1);
	  }
	
      }
      
      else {        
	
	//char temp[1024];           /* Parent */
	//ssize_t size;
	close(pipefd1[0]);          /* Close unused read end */
	close(pipefd2[1]);
	
	struct pollfd polls[2];
	
	polls[0].fd=0;
	polls[0].events = POLLIN | POLLHUP | POLLERR ;
	polls[1].fd=pipefd2[0];
	polls[1].events = POLLIN | POLLHUP |POLLERR;

	while(1)
	  {
	    
	    int val= poll(polls,2,-1);
	    
	    if(val<0)
	      {
		fprintf(stderr,"Error with poll()! \n");
		exit(1);
	      }
	
	    else 
	      {
		int i;
		char temp[1024];           /* Parent */
		ssize_t size;
		for(i=0;i<2;i++)
		  {
		    if(polls[i].revents & POLLIN)
		      {
			size=read(polls[i].fd,temp,sizeof(temp));
			
			
			if(size<0)
			  {
			    fprintf(stderr,"Unable to read! \n");
			    exit(1);
			  }
			
			else
			  {
			    int k=0;
			    int t=1;
			    int j;
			    for(;k<size;k++)
			      {
				//fprintf(stderr,"IN \n");
				if(temp[k]==0x04)
				  {
				    close(pipefd1[1]);
				    
				    t=0;
				    break;
				  }
				if(temp[k]=='\n' || temp[k]=='\r')
				  {
				    if(i==0)
				      write(pipefd1[1], "\n", 1);//temp[k]=='\n';
				    
				    
				    write(1, "\r\n", 2);
				    continue;
				  }
				
				if(temp[k]==0x03)
				  {
				    kill(cpid, SIGINT);
				  }
				
				if(i==0)
				  write(pipefd1[1], temp+k, 1);

				write(1, temp+k, 1);
				  }
			    
			    if(t==0)
			      {
				//reset();
				printf("SHELL EXIT SIGNAL=%d STATUS=%d \n\r", WEXITSTATUS(stat), WTERMSIG(stat));
				exit(0);
			      }
			    
			   
			      }
			  }
	      
		    
		    if(polls[i].revents & (POLLERR | POLLHUP))
		      {
			if(waitpid(cpid, &stat, 0)== -1)
			  {
			    fprintf(stderr,"Unable to execute waitpid! \n");
			    exit(1);
			  }
			printf("SHELL EXIT SIGNAL=%d STATUS=%d \n\r", WEXITSTATUS(stat), WTERMSIG(stat));
			exit(0);
		      }   
		  }
	      }
	  }
      }
    }
  
  else
    {
		


      char temp[1024];
      
      char c;
  
       
      ssize_t size;
      int j;
      while ((size = read(0, temp, sizeof(temp))) >= 0)
	{
	  
	  int i=0;
	  int t=1;
	  for(;i<size;i++)
	    {
	      
	      if(temp[i]==0x04)
		{
		  t=0;
		  break;
		}
	      if(temp[i]=='\n' || temp[i]=='\r')
		{
		  size+=1;
		  for(j=size-1;j>i;j--)    //check condition
		    {
		      temp[j]=temp[j-1];
		    }
		  
		  temp[i]='\r';
		  temp[i+1]='\n';
		  i++;
		}
	      
 
	      continue;
	    }
	  
	  if(t==0)
	    {
	      // reset();
	      return 0;
	    }
	  
	  write(1, temp, size);
	  
	  // size = read(0, &temp, sizeof(temp));
	  
	}
      
      
    }

    
  return 1;
      
}
  
