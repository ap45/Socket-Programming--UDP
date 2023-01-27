#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
// #include<sys/socket.h>
// #include<netinet/in.h>
// #include<arpa/inet.h>
// #include<netdb.h>
#include<string.h>
#include<winsock.h>
#include<winsock2.h>
#include<stdio.h>
void error(char *msg)
{
perror(msg);
exit(1);
}
int main(int argc, char *argv[])
{
int sock, length, n;
struct sockaddr_in server, from; 
struct hostent *hp; 
char buffer[256];
if (argc != 3)
{
printf("Usage: %s <server_name> <port>\n",argv[0]);
exit(1);
}
sock= socket(AF_INET, SOCK_DGRAM, 0);
if (sock < 0) error("socket");
server.sin_family = AF_INET;
hp = gethostbyname(argv[1]);
if (hp==0) error("Unknown host");
bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
server.sin_port = htons(atoi(argv[2]));
length=sizeof(struct sockaddr_in);
strcpy(buffer,"I have entered\n");
n=sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&server,length);
n=recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
write(1,"Server: ",8);
write(1,buffer,n);
printf("\n");
bzero(buffer,256);
fgets(buffer,255,stdin);
n=sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&server,length);
//CHATBOX
if(strncmp(buffer,"y",1)==0 || strncmp(buffer,"Y",1)==0)
{
while(1)
{
printf("You:");
bzero(buffer,256);
fgets(buffer,255,stdin);
n=sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&server,length);
if(strncmp(buffer,"bye",3)==0 || strncmp(buffer,"Bye",3)==0)
{
printf("The chat is closed . . bye \n");
goto p;
}
if(n < 0)
error("Sendto");
n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
if(n < 0)
error("recvfrom");
write(1,"Server: ",8);
write(1,buffer,n);
if(strncmp(buffer,"bye",3)==0)
{
printf("The chat is closed . . bye\n");
goto p;
}
}
}
//FILE TRANSFER
if(strncmp(buffer,"n",1)==0||strncmp(buffer,"N",1)==0)
{
n=recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
write(1,"Server: ",8);
write(1,buffer,n);
bzero(buffer,256);
fgets(buffer,255,stdin);
n=sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&server,length);
n=recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
write(1,"Server: ",8);
write(1,buffer,n);
if(strncmp(buffer,"Enter correct file name :",22)==0)
goto p;
while(1)
{
n=recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
if(strncmp(buffer,"stoppingopmpodwamo3;k213012k30kdpadkawpd0k231-k21k0fakd-kaw0k2313kpaokdadk;gafo;adkawoda;wf",50)==0)
goto p;
write(1,buffer,n);
}
}
p:close(sock);
exit(1);
}
