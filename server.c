#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 21

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
printf("FTP server waiting for connections...\n");// Accept incoming connections
socklen_t client_len = sizeof(struct sockaddr_in);
if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len)) == -1)
{
perror("Accept failed");
exit(1);
}
printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));// Send welcome message to client
char welcome_msg[] = "220 Welcome to FTP server\r\n";
bytes_sent = send(client_sock, welcome_msg, strlen(welcome_msg), 0);
if (bytes_sent == -1)
{
perror("Send failed");
exit(1);
}// Close the sockets
//ls command starting 
system("ls>a.txt");
char str[50];
char *msg="";
FILE *fp=fopen("a.txt","r");
int k,p;
int l1,l2;
msg = (char *)malloc(1);
while(fgets(str,50,fp)!=NULL){
	l1=strlen(str);
	l2=strlen(msg);
	msg=(char *)realloc(msg,l1+l2);
	strcat(msg,str);
}
send(client_sock,msg,strlen(msg),0);
close(client_sock);
close(server_sock);
return 0;
}
