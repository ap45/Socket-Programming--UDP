#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
// #include<sys/socket.h>
// #include<netinet/in.h>
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
int main(int argc,char *argv[])
{
FILE *fp;
int sock,length,fromlen,n;
struct sockaddr_in server;
struct sockaddr_in from;
char buf[1024],c=' ';
if(argc<2)
{
fprintf(stderr,"ERROR NO PORT provided \n");
exit(1);
}
sock=socket(AF_INET,SOCK_DGRAM,0);
if(sock<0)
error("Opening socket");
length=sizeof(server);
bzero(&server,length);
server.sin_family=AF_INET;
server.sin_addr.s_addr=INADDR_ANY;
server.sin_port=htons(atoi(argv[1]));
if(bind(sock,(struct sockaddr *)&server,length)<0)
error("binding");
fromlen=sizeof(struct sockaddr_in);
n=recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
write(1,"Client: ",8);
write(1,buf,n);
strcpy(buf,"Enter y for chatbox and n for file transfer");
n=sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&from,fromlen);
n=recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
//FILE TRANSFER
if(strncmp(buf,"n",1)==0 || strncmp(buf,"N",1)==0)
{
strcpy(buf,"Enter the file name in the server\n");
n=sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&from,fromlen);
bzero(buf,256);
n=recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
write(1,"Client: ",8);
write(1,buf,n);
for(int i=0;i<strlen(buf);i++)
if(buf[i]=='\n')
buf[i]='\0';
if((fp=fopen(buf,"r"))==NULL)
{
strcpy(buf,"Enter correct file name \n");
n=sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&from,fromlen);
printf("FILE NOT FOUND \n");
goto p;
}
strcpy(buf,"THE CONTENTS OF THE FILE ARE :\n");
n=sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&from,fromlen);
fseek(fp,0,SEEK_SET);
while((c=fgetc(fp))!=EOF)
{
bzero(buf,1024);
buf[0]=c;
for(int i=0;c!='\n';i++)
{
fscanf(fp,"%c",&c);
buf[i+1]=c;
}
n=sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&from,fromlen);
}
bzero(buf,255);
strcpy(buf,"stoppingopmpodwamo3;k213012k30kdpadkawpd0k231-k21k0fakd-kaw0k2313kpaokdadk;gafo;adkawoda;wf");
n=sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&from,fromlen);
printf("\nThe file has been displayed \n");
}
//CHATBOX
if(strncmp(buf,"Y",1)==0 || strncmp(buf,"y",1)==0)
{
while(1)
{
n=recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
if(n<0)
error("recvfrom");
write(1,"Client: ",8);
write(1,buf,n);
if(strncmp(buf,"bye",3)==0)
{
printf("The chat is closed . . bye \n");
goto p;
}
printf("You:");
bzero(buf,256);
fgets(buf,255,stdin);
n=sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&from,fromlen);
if(strncmp(buf,"bye",3)==0 || strncmp(buf,"Bye",3)==0)
{
printf("The chat is closed . . bye \n");
goto p;
}
if(n<0)
error("sendto");
}
}
p:close(sock);
exit(1);
}