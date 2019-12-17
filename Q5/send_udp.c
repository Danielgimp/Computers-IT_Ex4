#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#define MAXLINE 1024 

int main(int argc, char *argv[])
{
  int socket_fd;
  char buffer[MAXLINE]; //the server response string
  struct sockaddr_in  dest; //dest is sockaddr_in type var contains the info of the destination data (i.e IP Adress,port)
  struct hostent *hostptr; // hostptr is hostent type var contains the info of the source
  struct { char head; char* body; char tail; } msgbuf; //new data structure containing head,body and tail of a message
  char *name = "Daniel & Rinat"; //Sent string
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // uses AF_INET to manage address (IPv4), SOCK_DGRAM for UDP Comm, and 0 for default protocol> gives an int transport method
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */ //number of zeroes to be copied into dest from sizeof(dest)
  hostptr = gethostbyname(argv[1]); //sets the data of the host from argv[1] input (seond word in terminal) 
  dest.sin_family = (short) AF_INET; //sets the address type in dest which is AF_INET (IPv4)
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length); //copis the IPv4 address from (source) hostptr->h_addr to (dest address) (char *)&dest.sin_addr in hostptr->h_length size
  dest.sin_port = htons((u_short)0x3333); //sets the destination port (converts it from 16bit unsigned int to network type var)

  int n, len;

  msgbuf.head = '<'; //simple printf style output 
  msgbuf.body = "Does Nothing"; /* IMPORTANT! */ //returns my name
  msgbuf.tail = '>'; //simple printf style output 

  sendto(socket_fd,(const char *)name,strlen(name),MSG_CONFIRM,(struct sockaddr *)&dest,sizeof(dest)); //sends msgbuf, in size of msgbuf with method socket_fd to dest in 	size of dest
	n = recvfrom(socket_fd, (char *)buffer, MAXLINE,MSG_WAITALL,(struct sockaddr *)&dest,&len); 
    buffer[n] = '\0';
printf("The Server got the message! it is: \n %s",buffer); 

close(socket_fd);
  return 0; //stop 
}

