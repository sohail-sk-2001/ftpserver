#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 21

void list(int client_sock);
void quit(int client_sock);
void download(int client_sock);
void upload(int client_sock);


void handle_connection(int client_sock){
int bytes_sent;
char welcome_msg[] = "220 Welcome to FTP server\r\n";
bytes_sent = send(client_sock, welcome_msg, strlen(welcome_msg), 0);
if (bytes_sent == -1)
{
perror("Send failed");
exit(1);
}// Close the sockets
int n;
while(1){
recv(client_sock,&n,4,0);
printf("%d  //bug\n",n);
if(n==1){
list(client_sock);
}
if(n==2){
download(client_sock);
}
if(n==3){
upload(client_sock);
}
if(n==4){
quit(client_sock);
return;
}
}
}

void download(int client_sock){
printf("Waiting for upload\n");
char file_name[50];
char send_msg[50]="Ok";
char msg[1024];
if(recv(client_sock,file_name,sizeof(file_name),0)>0){
printf("File name is %s\n",file_name);
printf("File Received\n");
if(send(client_sock,send_msg,50,0)>0){
FILE *fw=fopen(file_name,"w");
recv(client_sock,msg,sizeof(msg),0);
fputs(msg,fw);
fclose(fw);
}
}
printf("File received from client");
return;
}

void upload(int client_sock){
printf("Waiting to Upload\n");
char file_name[50];
char str[50];
recv(client_sock,file_name,50,0);
printf("File %s is uploaded\n",file_name);
FILE *fr=fopen(file_name,"r");
int l1,l2;
char *msg="";
msg = (char *)malloc(1);
while(fgets(str,50,fr)!=NULL){
	l1=strlen(str);
	l2=strlen(msg);
	msg=(char *)realloc(msg,l1+l2);
	strcat(msg,str);
}
msg[strlen(msg)]='\0';
send(client_sock,msg,strlen(msg),0);
fclose(fr);
return;
}


void quit(int client_sock){
struct sockaddr_in client_addr;
socklen_t len=sizeof(client_addr);
getpeername(client_sock,(struct sockaddr*)&client_addr,&len);
printf("Client (%s:%d) is Disconnected\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
close(client_sock);
return;
}

void list(int client_sock){
printf("The List of files have been sent to the client\n");
system("ls>a.txt");
char str[50];
char *lmsg="";
FILE *fp=fopen("a.txt","r");
int k,p;
int l1,l2;
lmsg = (char *)malloc(1);
while(fgets(str,50,fp)!=NULL){
        l1=strlen(str);
        l2=strlen(lmsg);
        lmsg=(char *)realloc(lmsg,l1+l2);
        strcat(lmsg,str);
}
fclose(fp);
lmsg[strlen(lmsg)]='\0';
send(client_sock,lmsg,strlen(lmsg),0);
return;
}

int main()
{
int server_sock, client_sock, bytes_sent, bytes_recv;
struct sockaddr_in server_addr, client_addr;
char buffer[1024];// Create socket
if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
{
perror("Socket creation failed");
exit(1);
}// Bind socket to port
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
server_addr.sin_addr.s_addr = INADDR_ANY;
bzero(&(server_addr.sin_zero),8);
if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
{
perror("Socket binding failed");
exit(1);
}// Listen for incoming connections
if (listen(server_sock, 5) == -1)
{
perror("Listen failed");
exit(1);
}
printf("\nFTP server waiting for connections...\n");// Accept incoming connection
while(1){						    
socklen_t client_len = sizeof(struct sockaddr_in);
if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len)) == -1){
perror("Accept failed");
exit(1);
}
printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));// Send welcome message to client
handle_connection(client_sock);
}
close(server_sock);
return 0;
}

