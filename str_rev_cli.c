#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#define port 5000
#define maxlen 500
int main(){
	int client_socket;
	struct sockaddr_in servaddr;
	char buffer[maxlen];
	client_socket=socket(AF_INET,SOCK_STREAM,0);
	if(client_socket<0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(connect(client_socket,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		perror("Connection failed");
		close(client_socket);
		exit(1);
	}
	printf("Enter the string:\n");
	if(scanf("%s",buffer)<=0){
		perror("Input failed");
		close(client_socket);
		exit(1);
	}
	if(send(client_socket,buffer,sizeof(buffer),0)<0){
		perror("Send failed");
		close(client_socket);
		exit(1);
	}
	if(recv(client_socket,buffer,sizeof(buffer),0)<0){
		perror("Receive failed");
		close(client_socket);
		exit(1);
	}
	printf("The reversed string is:");
	printf("%s",buffer);
	printf("\n");
	close(client_socket);
	return 0;
}
