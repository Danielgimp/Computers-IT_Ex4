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
#define BUFFER_SIZE 1024
#define URL_LENGTH 1024
#define PROTOCOL_LENGTH 6

//a function that creates a substring
char* subString (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input); //get the input sting size

  if (offset + len > input_len) //throw an error if the wanted substring is bigger than the original string
  {
     return NULL;
  }

  strncpy(dest, input + offset, len); //using copying to make the sunstring and returning it
  return dest;
}
//a function that makes socket connection
int socket_connect(char *host, in_port_t port){
  struct hostent *hp;
  struct sockaddr_in addr;
  int on = 1, sock;     

  if((hp = gethostbyname(host)) == NULL){ //check if hostname string is null
    herror("Error in resolving host "); //an error type for typing unknown host
    exit(1); //quit
  }
  bcopy(hp->h_addr, &addr.sin_addr, hp->h_length); //copy the address to addr in h_lebgth size
  addr.sin_port = htons(port); //put the port number in addr
  addr.sin_family = AF_INET; //use ipv4 connection type
  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //create a socket connection in type of ipv4
  setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int)); //sets how sock will work with special flags which are IPPROTO_TCP  TCP_NODELAY

  if(sock == -1){ //if sock is -1 there was a problem in establishing the connection
    perror("There was an error establishing socket connection");
    exit(1);
  }
  
  if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){ // if connect is -1 there was a problem during the connection to the host
    perror("There was an error connectiong to the Host ");
    exit(1);

  }
  return sock;
}
 


int main(int argc, char *argv[]){
  //first part of the main is to parse the URL from the form <protocol>://<hostname>/<path> OR <protocol>://<hostname>:<port>/<path> ONLY...so for that there are all the vars that are needed
  int fd;
  char buffer[BUFFER_SIZE]; //string which will contain the data from http data and header
  char url[URL_LENGTH]; //will contain the url
  char Protocol[6]; //will contain the protocol (http or https)
  char Hostname[URL_LENGTH]; //will contain the hostname
  char path[URL_LENGTH]; //will contain the path
  char port[4]; //will contain the port
  int host_start; //stores host start point
  int path_start; //stores path start point
  int host_length; //stores the host length
  int path_length; //stores the path length
  int port_start; //stores port start point
  int port_length=0; //stores the port length
  int flag=0; //flag for a url of the form <protocol>://<hostname>:<port>/<path> to catch the port
  int flag__80=2; //flag to set the port to 80
  int flag_443=2; // flag to set the port to 443

  // throw an error if input is incorrect
  if(argc != 2){
    fprintf(stderr, "Usage: %s <protocol>://<hostname>/<path> OR <protocol>://<hostname>:<port>/<path> \n", argv[0]);
    exit(1); 
  }
  //copy terminal input to url string
  strcpy(url, argv[1]);
  //printf("URL Is:%s \n",url); prints the url - for debugging purposes
   for(int i=0;i<URL_LENGTH;i++) //iterate to catch all elements of the loop
  {
    if(url[i]==58 && i<=5) //here the code catches protocol i.e http or https 58=: in ascii
    {
      if(i==4) //meaning it is http since http is 4 chars
      {
        subString(url, 0, 4, Protocol); //copy http to protocol
        //printf("Protocol Is: %s \n",Protocol); prints the Protocol - for debugging purposes
        flag__80=1; //if we are here than the port is 80
        host_start=i+3; // skip the :// part of the url and set this location to be the host start
        i=i+3; //go to the location after the :// 
      }
      if(i==5)
      {
        subString(url, 0, 5, Protocol);
        //printf("Protocol Is: %s \n",Protocol); prints the Protocol - for debugging purposes
        flag__80=1; //if we are here than the port is 443
        host_start=i+3; // skip the :// part of the url and set this location to be the host start
        i=i+3; //go to the location after the :// 
      }
        
    }

    if(url[i]==58 && i>5) //that is the part that catches the url of the form <protocol>://<hostname>:<port>/<path>
    {
       if(i>5)
      {
        flag__80=0; //set the port 80 to be false (since there is <hostname>:<port> in the url)
        flag_443=0; //set the port 443 to be false (since there is <hostname>:<port> in the url)
        int j=i; //begin internal loop with j
        port_start=i+1; //define port start location
        while(flag==0) //flag is used to stop this loop
        {
          
          if(url[j]==47) //if url ==/ do the following
          {
           port_length=port_length-2; //define port length minus the : or / at the begging and end
           flag=1; //turn flag to one so that the loop would stop
          }
          j++; //as long as the loop won't stop uplift j
          port_length++; //as long as the loop won't stop uplift port_length

        }

      }
    }
    
    if(url[i]==47) //here i catch the "Path" of the url
    {
      path_start=i; //define path start to be i
     if(port_length>0)host_length=i-host_start-port_length-1; //in case there is url of the form <protocol>://<hostname>:<port>/<path> i need to trim the :<port> from the host
     else host_length=i-host_start-port_length; //the case where the url is in the form of <protocol>://<hostname>/<path>
    } 
    
    if(url[i]=='\0') //catch the end of the url
    {
      path_length=i-path_start; // set it to be the path end
      i=URL_LENGTH; //stop the loop from going all the way to 1024 and set it to 1024 right now (since we got the url)
    } 

  }
  //printf("host_length %d \n",host_length); prints the host_length - for debugging purposes
  //printf("path_start %d \n",path_start); prints the path_start - for debugging purposes
  //printf("path length %d \n",path_length); prints the path_length - for debugging purposes
  //printf("Port length %d \n",port_length); prints the port_length - for debugging purposes

  //set all url parameters\parts to different strings
  subString(url, host_start, host_length, Hostname); //get the hostname
  Hostname[host_length]='\0'; //close the hostname 
  //printf("Hostname is: %s \n",Hostname); prints the hostname - for debugging purposes
  if(path_length>0) //if there is a parh (and there always is since / is required)
  {
  subString(url, path_start, path_length, path); //get the path
  path[path_length]='\0'; //close the path
  //printf("path is: %s \n",path); prints the path - for debugging purposes

  }
  if(flag==1) //if statement that checks if the input was in form of <protocol>://<hostname>:<port>/<path>
  {
    subString(url, port_start, port_length, port); //get the port
    port[port_length]='\0'; //close the port
  }
  if(flag==0) //if the url wasn't in form of <protocol>://<hostname>:<port>/<path>
  {
    if(flag__80==1) strcpy(port,"80"); //flags that help define the port
    
    
  }
  //printf("The Port Is:%s \n",port);
        
  fd = socket_connect(Hostname, atoi(port)); //establish a socket connection
  write(fd, "GET /\r\n\r\n", strlen("GET /\r\n\r\n")); //write to the server "give me the html header and html data in size of html header" 
  bzero(buffer, BUFFER_SIZE); //initizlize the buffer for the html content
  
  while(read(fd, buffer, BUFFER_SIZE - 1) != 0){ //read data for the write request which is "give me the html header and html data in size of html header" into the buffer as long as it isn't null
    fprintf(stderr, "%s", buffer); //prints the html header and data to the UNIX console
    bzero(buffer, BUFFER_SIZE); //initizlize the buffer again
  }

  shutdown(fd, SHUT_RDWR); //close all the send and recieve operations by SHUT_RDWR
  close(fd); //close all operations in fd

  return 0;
}