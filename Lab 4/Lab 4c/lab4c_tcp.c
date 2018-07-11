//Name: Sparsh Arora
//UID: 804653078
//Email: sparsharora@g.ucla.edu


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mraa.h"
#include <math.h>
#include <string.h>
#include "mraa/aio.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>
#include <getopt.h>
#include <poll.h>

const int B = 4275;
const int R0 = 100000;
//typedef struct _aio * 	mraa_aio_context
char sc='f';
int lflag=0;
int log_fd=-1;
char* host;

mraa_aio_context adc_a0;
mraa_gpio_context gpio;




void shutdowns(int sfd)
{

time_t rawtime;
time_t last=0;
struct tm* timeinfo;

time(&rawtime);
timeinfo = localtime(&rawtime);

	if(lflag==1)
		dprintf(log_fd,"%02d:%02d:%02d SHUTDOWN\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	dprintf(sfd, "%02d:%02d:%02d SHUTDOWN\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	fprintf(stderr,"%02d:%02d:%02d SHUTDOWN\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

exit(0);
}

float get_temp(mraa_aio_context adc_0, char c)
{
int a;
a=mraa_aio_read(adc_0);

float R= 1023.0/a-1.0;
R=R0*R;

float Temp = 1.0/(log(R/R0)/B+1/298.15)-273.15;

if(c=='f')
	Temp=(Temp*1.8)+32;

return Temp;
}


void close_s()
{
	mraa_aio_close(adc_a0);
	mraa_gpio_close(gpio);
}

int
main(int argc, char **argv)
{
int port_num,id=0;	
char c;
int i=1;
int stop_f=1;
int sam_s=1;
//int log_fd=-1;
float tempr;
	while (1) 
		{	
			static struct option long_options[] ={
				{"id", 1, 0, 'i'},
				{"host", 1, 0, 'h'},
				{"log", 1, 0, 'l'},
				{0, 0, 0, 0}
			      };	
	
		        c = getopt_long(argc, argv, "i:h:l:",
					 long_options, NULL);	   
		       	if(c == -1)
			          break;
	
			switch (c)
			{
				case 'i':
					
					if(strlen(optarg)!=9)
					{
						fprintf(stderr,"Wrong input of id number!\n");
						exit(1);
					}
					else
					{
						id=atoi(optarg);
					
					}
					break;

				case 'h':
					host=optarg;
					break;	
				case 'l':
					lflag=1;
					log_fd=open(optarg,O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
					break;
				default:
						{
							fprintf(stderr,"Correct usage: ./lab4c_tcp [--id=] [--host=] [--log=] #port_number... \n");
				      			exit(1);
						}

			}
		}
	
while(1)
{
	if(argv[i][0]!='-')
		{
			//fprintf(stderr,"%s",argv[i][0]);
			port_num=atoi(argv[i]);
			break;
		}
	i++;
}
		
int sockfd;
  // portno=atoi(argv[1]);
struct sockaddr_in server_address;
if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
fprintf(stderr,"Unable to create a socket! \n");
exit(1);
}
struct hostent *server_name=gethostbyname(host);
                               
if (server_name == NULL) {
fprintf(stderr,"ERROR, no such host\n");
exit(1);
}

bzero((char*) &server_address, sizeof(server_address));
//server_address.sin_family = AF_INET;
bcopy((char*) server_name->h_addr, (char*)&server_address.sin_addr.s_addr, server_name->h_length);
server_address.sin_family = AF_INET;

server_address.sin_port = htons(port_num);
 
if(connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0 )
{
	fprintf(stderr,"Unable to connect to socket! \n");
	exit(1);
}
dprintf(sockfd,"ID=%d\n",id);

adc_a0 = mraa_aio_init(0);
gpio = mraa_gpio_init(3);
if (adc_a0 == NULL) {
	fprintf(stderr,"Couldn't initialize sensor!\n");
	exit(1);
		    }
if (gpio == NULL) {
	fprintf(stderr,"Couldn't initialize button!\n");
	exit(1);
		    }
mraa_gpio_dir(gpio,MRAA_GPIO_IN);

struct pollfd polls;
polls.fd=sockfd;
polls.events = POLLIN;
int val;


time_t rawtime;
time_t last=0;
struct tm* timeinfo;


time(&rawtime);
timeinfo = localtime(&rawtime);


tempr=get_temp(adc_a0, sc);
tempr=floor(tempr*10)/10.0;
if(stop_f==1 &&  rawtime-last>=sam_s){
	if(tempr<10.0)
	{
		if(lflag==1)
			dprintf(log_fd,"%02d:%02d:%02d 0%.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);

		dprintf(sockfd, "%02d:%02d:%02d 0%.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);
		fprintf(stderr, "%02d:%02d:%02d 0%.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);

	}
	else
	{
		if(lflag==1)
			dprintf(log_fd,"%02d:%02d:%02d %.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);

		dprintf(sockfd, "%02d:%02d:%02d %.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);
		fprintf(stderr, "%02d:%02d:%02d %.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);

	}
    
	last=rawtime;
	}




while(1)
{
	time(&rawtime);
	timeinfo = localtime(&rawtime);


	tempr=get_temp(adc_a0, sc);
	tempr=floor(tempr*10)/10.0;
	if(stop_f==1 &&  rawtime-last>=sam_s){
	if(tempr<10.0)
	{
		if(lflag==1)
			dprintf(log_fd,"%02d:%02d:%02d 0%.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);

		dprintf(sockfd, "%02d:%02d:%02d 0%.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);
		fprintf(stderr, "%02d:%02d:%02d 0%.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);

	}
	else
	{
		if(lflag==1)
			dprintf(log_fd,"%02d:%02d:%02d %.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);

		dprintf(sockfd, "%02d:%02d:%02d %.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);
		fprintf(stderr, "%02d:%02d:%02d %.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tempr);

	}
    
	last=rawtime;
	}

	if(mraa_gpio_read(gpio))
	{
		close_s();
		shutdowns(sockfd);
	}
	//fprintf(stderr,"1\n");
	val=poll(&polls,1,0);
	if(val<0)
	{
		fprintf(stderr,"Error with poll()! \n");
		close_s();		
		exit(1);
        }
	//fprintf(stderr,"2\n");

	if(polls.revents & POLLIN)
	{
		//fprintf(stderr,"3\n");

		char temp[1024]; 
		char *tok;
		ssize_t size;
		size=read(polls.fd,temp,sizeof(temp)-1);
		//fprintf(stderr,"here\n");
		//fprintf(stderr,"temp=%d\n", size);
		temp[size] = '\0';
		char null[2] = "\n";
		tok=strtok(temp,null);
		//size=strlen(tok);
		//fprintf(stderr,"%d\n",size);
		while(tok!=NULL){
		size = strlen(tok);
		if(size<0)									  {							  			    fprintf(stderr,"Unable to read! \n");
		    close_s();
		    exit(1);						    			  }
		
		if(size==3 && strncmp(tok,"OFF",3)==0)
		{
			if(lflag==1)
				dprintf(log_fd, "OFF\n");
			close_s();
			shutdowns(sockfd);
		}
		else if(size==4 && strncmp(tok,"STOP",4)==0)
		{
			stop_f=0;
			if(lflag==1)
				dprintf(log_fd,"STOP\n");
		}
		else if(size==5 && strncmp(tok,"START",5)==0)
		{
			stop_f=1;
			if(lflag==1)
				dprintf(log_fd,"START\n");
		}
		else if(size==7 && (strncmp(tok,"SCALE=F",7)==0 || strncmp(tok,"SCALE=C",7)==0))
		{
			if(tok[6]=='C')
			{
				if(lflag==1)
					dprintf(log_fd, "SCALE=C\n");
				sc='c';
			}
			else if(tok[6]='F')
			{
			if(lflag==1)
				dprintf(log_fd,"SCALE=F\n");
			sc='f';
			}
			else
			{
				close_s();
				fprintf(stderr,"Wrong Input!\n");
				exit(1);
			}
		}
		else if(size==8 && strncmp(tok,"PERIOD=",7)==0)
		{
			int new;
			new=atoi(&tok[7]);
			if(lflag==1)
				dprintf(log_fd, "PERIOD=%d\n",new);
			sam_s=new;
			if(sam_s<=0)
			{
				close_s();
				fprintf(stderr,"Invalid Period!\n");
				exit(1);
			}
		}
		else
		{
			close_s();
			fprintf(stderr,"Invalid Command/Input!\n");
			exit(1);
		}
		tok=strtok(NULL,"\n");
		}
	}
} 		

close_s();
return 0;

}

