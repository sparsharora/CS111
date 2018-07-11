//NAME: Sparsh Arora
//UID: 804653078
//EMAIL: sparsharora@g.ucla.edu


#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <poll.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mcrypt.h>

struct termios restore;
MCRYPT td1, td2;

void reset()
{
  tcsetattr (STDIN_FILENO, TCSANOW, &restore);
  mcrypt_generic_deinit(td1);
  mcrypt_generic_deinit(td2);
  mcrypt_module_close(td1);
  mcrypt_module_close(td2);

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



int
main(int argc, char **argv)
{

  //MCRYPT td1, td2;
  int c, lflag=0, lfd=-1;
  int portno, eflag=0, kfd; 
 int pflag=0;
  
  /*if(argc < 3)
    {
      fprintf(stderr,"Correct usage: lab1b-client [--port=] [--log=] ... \n");
      exit(1);
    }
  */
  while (1) {
    static struct option long_options[] = {
      {"port", 1, 0, 'p'},
      {"log", 1, 0, 'l'},
      {"encrypt", 1, 0, 'e'},
      {0, 0, 0, 0}
    };
    
    c = getopt_long(argc, argv, "p:l:e:",
		    long_options, NULL );
    if(c == -1)
      break;
        
    switch (c) {
    case 'p':
      portno=atoi(optarg);
      pflag=1;
      break;
    case 'l':
      lfd=creat(optarg, S_IRWXU);
      lflag=1;
      if(lfd<0)
	{
	  fprintf(stderr,"Unable to create file for log \n");
	  exit(1);
	}
      break;
    case 'e':
      eflag=1;
      kfd=open(optarg, O_RDONLY);
      break;
    default:
      {
	fprintf(stderr,"Correct usage: lab1b-client [--port=] [--log=] ... \n");
	exit(1);
      }
    }
  }
  
  if(pflag==0){
    fprintf(stderr,"Correct usage: lab1b-client [--port=] [--log=] ... \n");
    exit(1);
    }
  if(!set_istrip(STDIN_FILENO))
    {
      perror ("error in tcgetattr");
      return 1;
    }
 
  

  int sockfd, n;
  // portno=atoi(argv[1]);
  struct sockaddr_in server_address;

  if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      fprintf(stderr,"Unable to create a socket! \n");
      exit(1);
    }

  struct hostent *server_name=gethostbyname("localhost");
  
  if (server_name == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(1);
  }

  

  bzero((char*) &server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  bcopy((char*) server_name->h_addr, (char*)&server_address.sin_addr.s_addr, server_name->h_length);
  server_address.sin_port = htons(portno);

  if(connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0 )
    {
      fprintf(stderr,"Unable to connect to socket! \n");
      exit(1);
    }

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
  
  
  

  struct pollfd polls[2];
  
  polls[0].fd=0; //read from keyboard
  polls[0].events = POLLIN | POLLHUP | POLLERR ;
  polls[1].fd=sockfd;   //read from socket
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
	  int i=0,n;
	  char temp[1024];
	  ssize_t size;
	  for(;i<2;i++)
	    {
	      if(polls[i].revents & POLLIN)
		{
		  size = read(polls[i].fd, temp, sizeof(temp));
		  if(size<0)
		    fprintf(stderr,"Unable to read in client! \n");
		  else if(size==0)
		    exit(0);
		  else
		    {
		      if(i==0)
			dprintf(lfd, "SENT %d bytes: ", size);
		      else
			dprintf(lfd, "RECIEVED %d bytes: ", size);
                         
		      if(i==1 && eflag==1)
			{
			  mdecrypt_generic(td2, temp, size);
			}
		      if(i==1 && lflag==1)
			{
			  write(lfd, temp, size);
			  dprintf(lfd, "\n");
			}
		      int k=0,t=0;
		      char sp[1]; 
		      sp[0]='\n';
		      for(k=0;k<size;k++)
			{
			  if(temp[k]=='\r' || temp[k]=='\n')
			    {
			      if(i==0)
				{
				  if(eflag==1)
				    {
				      mcrypt_generic (td1, sp, 1);
				    }
				  if(lflag==1)
				    {
				      write(lfd, sp, 1);
				      dprintf(lfd,"\n");
				    }
				  write(sockfd, sp, 1);
				}
	
			      write(1, "\r\n", 2);
			    }
			  else
			    {
				 
			      write(1, temp+k, 1);
			      
			      if(i==0)
				{
				  if(eflag==1)
				    {
				      mcrypt_generic(td1, temp+k, 1);
				    }
				  if(lflag==1)
				    {
				      write(lfd, temp+k, 1);
				      dprintf(lfd, "\n");
				    }
				      write(sockfd, temp+k, 1);
				}
			    }	
			  
			  
			}
		      
		    }
		}
	    }
	}
    }
}

