//NAME: Sparsh Arora                                                                                                                                                                                        
//UID: 804653078                                                                                                                                                                                            
//EMAIL: sparsharora@g.ucla.edu 


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mcrypt.h>
#include <sys/stat.h>
#include <fcntl.h>


int stats;
int sockfd, newsockfd, clilen;
MCRYPT td1, td2;

void resetexit()
{
  shutdown(sockfd,2);
  mcrypt_generic_deinit(td1);
  mcrypt_generic_deinit(td2);
  mcrypt_module_close(td1);
  mcrypt_module_close(td2);
}



void signal_handler(int signum)
{
  if(signum==SIGPIPE)
    {
      printf("SHELL EXIT SIGNAL=%d STATUS=%d \n\r", WEXITSTATUS(stats), WTERMSIG(stats));
      exit(0);
    }

}




int main(int argc, char *argv[])
{
  int c,eflag=0, kfd;
  int portno;
  int pflag=0;
  atexit(resetexit);
 
  while (1) {
    static struct option long_options[] = {
      {"port", 1, 0, 'p'},
      {"encrypt", 1, 0, 'e'},
      {0, 0, 0, 0}
    };
    
    c = getopt_long(argc, argv, "p:e:",
		    long_options, NULL );
    if(c == -1)
      break;
        
    switch (c) {
    case 'p':
      pflag=1;
      portno=atoi(optarg);
      break;
    case 'e':
      eflag=1;
      kfd=open(optarg, O_RDONLY);
      break;
    default:
    
      {
	fprintf(stderr,"Correct usage: lab1a [--shell] ... \n");
	exit(1);
      }
    }
  }

  if (pflag==0)
    {
      fprintf(stderr,"Correct usage: lab1a [--shell] ... \n");
      exit(1);
    }
  //int sockfd, newsockfd, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0) 
    error("ERROR on accept");



  char* IV= "AAAAAAAAAAAAAAAA";
  char key[20];
  int keysize;

  if(eflag==1)
    {
      td1 = mcrypt_module_open("twofish", NULL, "cfb", NULL);
      if (td1==MCRYPT_FAILED) {
        fprintf(stderr,"Unable to find algorithm for encryption!! \n");
        exit(1);
      }
      td2 = mcrypt_module_open("twofish", NULL, "cfb", NULL);
      if (td2==MCRYPT_FAILED) {
        fprintf(stderr,"Unable to find algorithm for encryption! \n");
        exit(1);

      }

      n=read(kfd, key, 16);
      if(n<0)
        {
          fprintf(stderr,"Unable to read the key! \n");
          exit(1);
        }

      // keysize=mcrypt_get_key_size();                                                                                                                                                                     
      keysize=16;
      n=mcrypt_generic_init( td1, key, keysize, IV);
      if (n<0) {
        fprintf(stderr,"Unable to initialize the encryption! \n");
        exit(1);
      }

      n=mcrypt_generic_init( td2, key, keysize, IV);
      if (n<0) {
        fprintf(stderr,"Unable to initialize the encryption! \n");
        exit(1);
      }
    }


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
    // pipefd1 socket-Read, shell-Write
    // pipefd2 socket-Write, shell-Read
    dup2(pipefd1[0],0);
    dup2(pipefd2[1],1);
    dup2(pipefd2[1],2);
    
    
    
    char* myargs[2];
    //    fprintf(stderr, "SHELL \n");
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
    
    polls[0].fd=newsockfd;
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
	    int i,n;
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
		    if(i==0 && size==0)
		      kill(cpid, SIGTERM);
		    
		    else
		      {
			if(i==0 && eflag==1)
			  mdecrypt_generic(td2, temp, size);
			int k=0;
			for(;k<size;k++)                                                                                                                            
			  {
			    if(temp[k]==0x04)
			      {
				close(pipefd1[1]);
			      }
			    else if(temp[k]==0x03)
			      {
				kill(cpid, SIGINT);
			      }
			  }

			if(i==0)
			  {
			    /*fprintf(stderr,"CONNECTED \n");
			    if(eflag==1)
			    mdecrypt_generic(td2, temp, size);*/
			    n=write(pipefd1[1], temp, size);
			  }
			else
			  {
			    if(eflag==1)
			      mcrypt_generic(td1, temp, size);
			    write(newsockfd, temp, size);
			  }
		      }
			    
			   
		  }
	      
	  
		
		if(polls[i].revents & (POLLERR | POLLHUP))
		  {
		    if(waitpid(cpid, &stats, 0)== -1)
		      {
			fprintf(stderr,"Unable to execute waitpid! \n");
			exit(1);
		      }
		    printf("SHELL EXIT SIGNAL=%d STATUS=%d \n\r", WEXITSTATUS(stats), WTERMSIG(stats));
		    exit(0);
		  }   
	      }
	  }
      }
  }


}
