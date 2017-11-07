#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netdb.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

#define RHOST "NIL"
#define PORT 5000
#define SEC_TIMEOUT 0
#define USEC_TIMEOUT 20000


int socktimeout(int s);

int portassign = PORT;

int main(int argc, char*argv)
{
printf("Staring Server...\n");
printf("Connection Port %d\n",PORT);
int sck;
int pid;
int length;
char msg[1024];
struct sockaddr_in r_sa = {0};//store reciever details
sck = socket(AF_INET,SOCK_DGRAM, PF_UNSPEC);//create a socket
r_sa.sin_family = AF_INET;
r_sa.sin_addr.s_addr = INADDR_ANY;
r_sa.sin_port = htons(PORT);//converts integer port to network port
bind(sck,(struct sockaddr*)&r_sa,sizeof(r_sa));//bind function assigns address to socket s 
listen(sck,10);//keeps listening to the socket to accept connection
length = sizeof(r_sa);
int status;
pid=0;
while(1)
{
int ret = socktimeout(sck);
if(ret==1)
{
recvfrom(sck,msg,1024,0,(struct sockaddr*)&r_sa,&length);
portassign++;
sprintf(msg,"%d",portassign);
sendto(sck,msg,1024,0,(struct sockaddr*)&r_sa,length);
pid = fork();
if(pid==0)
{
execl("/home/nil/OS/Project1/server","./server" , msg, NULL);
}
}
if(pid!=0)
{
ret = waitpid(WAIT_ANY,&status,WNOHANG);
if(ret==-1)
{
printf("NO CLIENTs \nSERVER SHUTTING DOWN...\n");
break;
}
}
}

close(sck);


}


int socktimeout(int s)
{
int ret;
fd_set mask;
struct timeval tout;
tout.tv_sec=SEC_TIMEOUT;
tout.tv_usec=USEC_TIMEOUT;
FD_ZERO(&mask);
FD_SET(s,&mask);
ret = select(32,&mask,0,0,&tout);
return ret;
}

