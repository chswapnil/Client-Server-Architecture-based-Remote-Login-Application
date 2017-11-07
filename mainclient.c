#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netdb.h>// stores pointer to hostname, addresstype(h_addr), length of address(h_length) and address(h_addr)
#include<netinet/in.h>//sotes protocol family such as sin_family, sin_port, sin_addr, etc
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<arpa/inet.h>//definition for internet operation such as htons, htonl,etc
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

#define RHOST "NIL"
#define TIMEOUT 12
#define PORT 5000

void getRequest();


int portassign = PORT;

int main(int argc, char*argv)
{
getRequest();
char msg[1024];
sprintf(msg,"%d",portassign);
int pid = fork();
if(pid==0)
{
execl("/home/nil/OS/Project1/client","./client" , msg, NULL);
}
else
{
wait(0);
}
printf("CLIENT %d SHUTTING DOWN\n",portassign-PORT);
}

void getRequest()
{
struct sockaddr_in sa = {0};
struct hostent *hp;//In netdb header network database
int sck;
int length;
char msg[1024];
hp = gethostbyname(RHOST);
bcopy((char*)hp->h_addr,(char*)&sa.sin_addr, hp->h_length);//copies hosts address into sa's address
sa.sin_family = hp->h_addrtype;//stores type of family into address type
sa.sin_port = htons(portassign);//converts integer port to network type port and stores in sa's port
sck = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);//creates a socket
length = sizeof(sa);
sendto(sck,msg,1024,0,(struct sockaddr*)&sa,length);
recvfrom(sck,msg,1024,0,(struct sockaddr*)&sa,&length);
close(sck);
portassign = atoi(msg);

}
