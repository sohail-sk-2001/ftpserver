#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 21
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
}// Receive welcome message from server

bytes_recv = recv(client_sock, buffer, 1024, 0);
if (bytes_recv == -1)
{
perror("Receive failed");
exit(1);
}
buffer[bytes_recv] = '\0';
printf("%s", buffer);
bzero(buffer,sizeof(buffer));
int n;
printf("Enter your choice:\n1.ls\n2.Read\n3.Write\n");
scanf("%d",&n);
if(send(client_sock,&n,4,0)>0){
recv(client_sock,buffer,1024,0);
printf("The list of files in server:\n %s",buffer);
}
// Close the socket
close(client_sock);
return 0;
}
