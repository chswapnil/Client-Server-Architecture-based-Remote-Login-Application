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
#define TIMEOUT 12
#define PORT 5000

void sendTCP(int s, char*buff, int len);
int receiveTCP(int s,char*buff, int len);
int socktimeout(int s);
int serverSetup();
void serverHandshake(int s);
void doOperation(char*buff);

//Global Variables
struct sockaddr_in sock;
struct sockaddr_in sa = {0};//store sender details
struct sockaddr_in r_sa = {0};//store reciever details
int sck;
int size = 1024;
int portassign=PORT;

int main(int argc, char*argv[])
{

portassign = atoi(argv[1]);
printf("Port Assigned %d\n",portassign);
int length;
char msg[1024];
char response[size];

int s = serverSetup();
serverHandshake(s);




sprintf(response,"Ack");
usleep(500000);
while(1)
{
int a = receiveTCP(s,(char*)msg,1024);
if(a==1)
{
sendTCP(s,(char*)response,1024);
}
else if(a==2)
{
printf("Server->Client%d:Ending Connection...\n",portassign-PORT);
break;
}
}
close(s);
exit(0);
}


void serverHandshake(int s)
{
char msg[1024];
char response[1024];
int length;
//Code for handshake
length = sizeof(sock);
recvfrom(s,msg,1024,0,(struct sockaddr*)&sock,&length);
printf("Client %s\n",msg);
sprintf(response,"OK");
sendto(s,response,1024,0,(struct sockaddr*)&sock,length);

}


//Setting up the server
int serverSetup()
{

sck = socket(AF_INET,SOCK_DGRAM, PF_UNSPEC);//create a socket
sa.sin_family = AF_INET;
sa.sin_addr.s_addr = INADDR_ANY;
sa.sin_port = htons(portassign);//converts integer port to network port
bind(sck,(struct sockaddr*)&sa,sizeof(sa));//bind function assigns address to socket s 
listen(sck,10);//keeps listening to the socket to accept connection
return sck;
}





void sendTCP(int s, char*buff, int len)
{
sendto(s,buff,len,0,(struct sockaddr*)&sock,sizeof(sock));//sends msg to a socket. Here destination addr is mentioned
}


int receiveTCP(int s,char*buff,int len)
{
char cmsg[1024];
int sz = sizeof(sock);
int flag;
flag = socktimeout(s);
if(flag==1)
{
recvfrom(s,cmsg,len,0,(struct sockaddr*)&sock, &sz);//recieves from the reciever. Here message source addr is mentioned

printf("Client%d: %s",(portassign-PORT),cmsg);
if(strcmp("quit\n",cmsg)==0)
{
return 2;
}
doOperation(cmsg);
return 1;
}
else if(flag!=1&&flag>=0)
{
printf("Timeout->Client%d\n",portassign-PORT);
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


void doOperation(char*buff)
{
//Refines the values
int i=0;
if((int)strlen(buff)>=2)
{
while(1)
{
if(buff[i]==' ')
{i++;}
else
{break;}
}
buff = &buff[i];
}

int res;
if(strcmp("ls\n",buff)==0)
{
res = fork();
if(res==0)
{
printf("Server->Client%d:\n",portassign-PORT);
execl("/bin/ls","ls",NULL,NULL);
}
else
{
wait(0);
}
}
else if(strcmp("pwd\n",buff)==0)
{
printf("Server->Client%d:\n",portassign-PORT);
res = fork();
if(res==0)
{execl("/bin/pwd","pwd",NULL,NULL);}
else
{wait(0);}
}
else if(strcmp("do nothing\n",buff)==0)
{
printf("Server->Client%d:\nDoing Nothing\n",portassign-PORT);
}
else if(buff[0]=='c'&&buff[1]=='d')
{
printf("Server->Client%d:\n",portassign-PORT);
buff = &buff[2];
if((int)strlen(buff)>=2)
{
while(1)
{
if(buff[i]==' ')
{i++;}
else
{break;}
}
buff = &buff[i];
}
char m[128];
if(buff[0]=='/')
{sprintf(m,"%s",buff);}
else
{sprintf(m,"/%s",buff);}
m[((int)strlen(m))-1]='\0';
res = fork();
if(res==0)
{
if(chdir(m)!=-1)
{execl("/bin/pwd","pwd",NULL,NULL);}
else
{printf("Directory not found\n");}
}
else
{wait(0);}
}

}

















