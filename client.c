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

int clientSetup();
void sendTCP(int s, char*buff, int len);
int receiveTCP(int s,char*buff, int len);
int socktimeout(int s);
void clientHandshake(int s);


//Global variables
struct sockaddr_in sock = {0};//In netinet/in header
struct sockaddr_in sa = {0};
struct hostent *hp;//In netdb header network database
int sck;
int size = 1024;
int portassign = PORT;




int main(int argc, char*argv[])
{
printf("Starting Client...\n");
portassign = atoi(argv[1]);
printf("Port Assigned %d\n",portassign);
char msg[1024];//To store the message
char recv[1024];//variable to store the recieved character message
char temp[1024];
int s = clientSetup();
clientHandshake(s);

while(1)
{
printf("Enter command: ");//Enter message you want to sent
fgets(msg,1024,stdin);
sock = sa;
sendTCP(s,(char*)&msg,1024);
if(strcmp(msg,"quit\n")==0)
{
printf("Client%d Ending Connection...\n",portassign-PORT);
break;
}
int a = receiveTCP(s,(char*)&recv,1024);

}
close(s);//closes the socket
exit(0);
}






//Method for handshaking
void clientHandshake(int s)
{
char msg[1024];
//Handshake between client and server
int length = sizeof(sa);
sprintf(msg,"Requesting Connection...");
printf("%s\n",msg);
sendto(s,msg,1024,0,(struct sockaddr*)&sa,length);
recvfrom(s,msg,1024,0,(struct sockaddr*)&sa,&length);
char temp[3];
sprintf(temp,"OK");
if(strcmp(msg,temp)==0)
{
printf("Server: %s\n",msg);
}
}

//Method to setup the client
int clientSetup()
{
hp = gethostbyname(RHOST);//gets hosts addresss, name, and address type and stores in structure hp
bcopy((char*)hp->h_addr,(char*)&sa.sin_addr, hp->h_length);//copies hosts address into sa's address
sa.sin_family = hp->h_addrtype;//stores type of family into address type
sa.sin_port = htons(portassign);//converts integer port to network type port and stores in sa's port
sck = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);//creates a socket
return sck;
}


//Method to send the value
void sendTCP(int s, char*buff, int len)
{
sendto(s,buff,len,0,(struct sockaddr*)&sock,sizeof(sock));//sends msg to a socket. Here destination addr is mentioned
}


//Method to recieve the value
int receiveTCP(int s,char*buff,int len)
{
int sz = sizeof(sock);
int flag;
flag = socktimeout(s);
if(flag==1)
{
recvfrom(s,buff,len,0,(struct sockaddr*)&sock, &sz);//recieves from the reciever. Here message source addr is mentioned
return 1;
}
else if(flag!=1&&flag>=0)
{
printf("Socket Timeout\n");
return -1;
}
}

int socktimeout(int s)
{
int ret;
fd_set mask;
struct timeval tout;
tout.tv_sec=TIMEOUT;
tout.tv_usec=0;
FD_ZERO(&mask);
FD_SET(s,&mask);
ret = select(32,&mask,0,0,&tout);
return ret;
}









