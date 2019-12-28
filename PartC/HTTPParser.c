#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#define URL_LENGTH 1024

char* subString (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input);

  if (offset + len > input_len)
  {
     return NULL;
  }

  strncpy(dest, input + offset, len);
  return dest;
}

int main(int argc, char *argv[]){
  char url[URL_LENGTH];
  char Protocol[6];
  char Hostname[URL_LENGTH];
  char path[URL_LENGTH];
  char port[4];
  int host_start;
  int path_start;
  int host_length;
  int path_length;
  int port_start;
  int port_length=0;
  int flag=0;
  int flag__80=2;
  int flag_443=2;

  if(argc != 2){
    fprintf(stderr, "Usage: %s <protocol>://<hostname>/<path> OR <protocol>://<hostname>:<port>/<path> \n", argv[0]);
    exit(1); 
  }
  strcpy(url, argv[1]);
  printf("URL Is:%s \n",url);
   for(int i=0;i<URL_LENGTH;i++) //iterate to catch http or https protocols
  {
    if(url[i]==58 && i<=5)
    {
      if(i==4)
      {
        subString(url, 0, 4, Protocol);
        printf("Protocol Is: %s \n",Protocol);
        flag__80=1;
        host_start=i+3;
        i=i+3;
      }
      if(i==5)
      {
        subString(url, 0, 5, Protocol);
        printf("Protocol Is: %s \n",Protocol);
        flag_443=1;
        host_start=i+3;
        i=i+3;
      }
        
    }

    if(url[i]==58 && i>5)
    {
    	 if(i>5)
      {
      	flag__80=0;
      	flag_443=0;
      	int j=i;
      	port_start=i+1;
      	while(flag==0)
      	{
      		
      		if(url[j]==47)
      		{
      		 port_length=port_length-2;
      		 flag=1;
      		}
      		j++;
      		port_length++;

      	}

      }
    }
    
    if(url[i]==47)
    {
      path_start=i;
      if(port_length>0)host_length=i-host_start-port_length-1;
      else host_length=i-host_start-port_length;
     
    } 
    
    if(url[i]=='\0')
    {
      path_length=i-path_start;
      i=URL_LENGTH;
    } 

  }
  printf("host_length %d \n",host_length);
  printf("path_start %d \n",path_start);
  printf("path length %d \n",path_length);
  printf("Port length %d \n",port_length);
  subString(url, host_start, host_length, Hostname);
  Hostname[host_length]='\0';
  printf("Hostname is: %s \n",Hostname);
  if(path_length>0)
  {
  subString(url, path_start, path_length, path);
  path[path_length]='\0';
  printf("path is: %s \n",path);

  }
  //strcpy(port,"80");
  //strcpy(port,"443");

  if(flag==1)
  {
  	subString(url, port_start, port_length, port);
  	port[port_length]='\0';
  }
  if(flag==0) 
  {
  	if(flag__80==1) strcpy(port,"80");
  	if(flag_443==1) strcpy(port,"443");
  	//strcpy(port,"0");
  }
  printf("The Port Is:%s \n",port);
  
 
  return 0;
  }
