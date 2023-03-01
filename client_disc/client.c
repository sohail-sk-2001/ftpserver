#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 21

void put(int client_sock);
void get(int client_sock);

void handle_connection(int client_sock){
int bytes_recv;
char buffer[1024];
bytes_recv = recv(client_sock, buffer, 1024, 0);
if (bytes_recv == -1)
{
perror("Receive failed");
exit(1);
}
buffer[bytes_recv] = '\0';
printf("%s", buffer);
bzero(buffer,sizeof(buffer));
while(1){
int n;
printf("\n\n\nEnter your choice:\n1.List\n2.Upload\n3.Download\n4:Quit\n");
scanf("%d",&n);
if(n==1){
if(send(client_sock,&n,4,0)>0){
recv(client_sock,buffer,1024,0);
printf("The list of files in server:\n %s",buffer);
}}
else if(n==2){
if(send(client_sock,&n,4,0)>0){
put(client_sock);
}
}
else if(n==3){
if(send(client_sock,&n,4,0)>0){
get(client_sock);
}
}
else if(n==4){
if(send(client_sock,&n,4,0)>0){
printf("Client Disconnecting From Server\n");
close(client_sock);
exit(0);
}}
else{
printf("Wrong Input......Please Try Again\n");
}
}
}


void put(int client_sock){
printf("Enter name of the file to be uploaded\n");
char file_name[50];
char recv_msg[50];
char str[50];
scanf("%s",file_name);
if(send(client_sock,file_name,strlen(file_name),0)>0){
recv(client_sock,recv_msg,50,0);
if((strncmp(recv_msg,"Ok",2))==0){
FILE *fr=fopen(file_name,"r");
int l1,l2;
char *msg="";
msg = (char *)malloc(1);
while(fgets(str,50,fr)!=NULL){
	l1=strlen(str);
	l2=strlen(msg);
	msg=(char*)realloc(msg,l1+l2);
	strcat(msg,str);
}
msg[strlen(msg)]='\0';
send(client_sock,msg,strlen(msg),0);
fclose(fr);
}
}
printf("File has been Uploaded\n");
return;
}


void get(int client_sock){
printf("Enter the name of the file to be donwloaded\n");
char file_name[50];
scanf("%s",file_name);
send(client_sock,file_name,strlen(file_name),0);
char msg[1024];
FILE *fw=fopen(file_name,"w");
recv(client_sock,msg,sizeof(msg),0);
fputs(msg,fw);
fclose(fw);
printf("File received from server\n");
return;
}



int main(){
int client_sock, bytes_sent, bytes_recv;
struct sockaddr_in server_addr;
char buffer[1024];// Create socket
if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
{
perror("Socket creation failed");
exit(1);
}// Set server address
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
bzero(&(server_addr.sin_zero), 8);// Connect to server
if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
{
perror("Connect failed");
exit(1);
}
handle_connection(client_sock);
return 0;
}
