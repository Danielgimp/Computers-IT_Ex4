#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
 
/*
void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s: ", str2);
  -- port: sin->sin_port (host integer type) 
  -- IP: sin->sin_addr (IP in dotted-decimal notation) 
  printf("\n");
}
*/
 
int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from; //define s_in and from as network type data structures
  struct { char head; u_long  body; char tail;} msg; //new data structure containing head,body and tail of a message
 
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // uses AF_INET to manage address (IPv4), SOCK_DGRAM for UDP Comm, and 0 for default protocol> gives an int transport method
 
  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */ //number of zeroes to be copied into s_in from sizeof(s_in) 
 
  s_in.sin_family = (short)AF_INET; //sets the address type in dest which is AF_INET (IPv4)
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
  s_in.sin_port = htons((u_short)0x3333); //sets the destination port (converts it from 16bit unsigned int to network type var)
 
  	void printsin(struct sockaddr_in *sin, char *pname, char* msg)
  	{
   	 char ipaddr[INET_ADDRSTRLEN];
	 char portnum[INET_ADDRSTRLEN];
	 inet_ntop(AF_INET, &(sin->sin_addr), ipaddr, INET_ADDRSTRLEN);
	 printf("%s \n",pname);
	 printf("%s ip=%s , port=%hu \n",msg,ipaddr,ntohs(s_in.sin_port));
	 printf("Client Port: %hu\n", ntohs(s_in.sin_port));
	}

 
  fflush(stdout); //flush the output of the console
 
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));
 
  for(;;) {
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
    printsin( &from, "recv_udp: ", "Packet from:");
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); 
    fflush(stdout);
  }
   
  return 0;
}
