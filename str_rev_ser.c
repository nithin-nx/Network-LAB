#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define port 5000
#define maxlen 500
int main(){
	int server_socket,client_socket;
	struct sockaddr_in servaddr;
	char buffer[maxlen];
	int len,i;
	server_socket=socket(AF_INET,SOCK_STREAM,0);
	if(server_socket<0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(server_socket,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		perror("Bind failed");
		close(server_socket);
		exit(1);
	}
	if(listen(server_socket,5)<0){
		perror("Listen failed");
		close(server_socket);
		exit(1);
	}
	printf("Server is waiting for a connection......\n");
	client_socket=accept(server_socket,NULL,NULL);
	if(client_socket<0){
		perror("Accept failed");
		close(server_socket);
		exit(1);
	}
	if(recv(client_socket,buffer,sizeof(buffer),0)<0){
		perror("Receive failed");
		close(client_socket);
		close(server_socket);
		exit(1);
	}
	printf("The string received from the client is:\n%s",buffer);
	len=strlen(buffer);
	for(i=0;i<len/2;i++){
		char temp=buffer[i];
		buffer[i]=buffer[len-i-1];
		buffer[len-i-1]=temp;
	}
	printf("\nThe string is reversed and send to the client...\n");
	printf("\n");
	if(send(client_socket,buffer,sizeof(buffer),0)<0){
		perror("Send failed");
		close(client_socket);
		close(server_socket);
		exit(1);
	}
	close(client_socket);
	close(server_socket);
	return 0;
}
