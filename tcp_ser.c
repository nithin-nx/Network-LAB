#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define port 5000
#define maxline 500
int main(){
	int serv_sock,cli_sock;
	char buffer[maxline];
	struct sockaddr_in servaddr;
	int len,i;
	serv_sock=socket(AF_INET,SOCK_STREAM,0);
	if(serv_sock<0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(serv_sock,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		perror("Bind failed");
		close(serv_sock);
		exit(1);
	}
	if(listen(serv_sock,5)<0){
		perror("Listen failed");
		close(serv_sock);
		exit(1);
	}
	printf("Server is waiting for incoming connection.....\n");
	cli_sock=accept(serv_sock,NULL,NULL);
	if(cli_sock<0){
		perror("Accept failed");
		close(serv_sock);
		exit(1);
	}
	if(recv(cli_sock,buffer,sizeof(buffer),0)<0){
		perror("Receive failed");
		close(cli_sock);
		close(serv_sock);
		exit(1);
	}
	printf("Message from client :\n");
	for(i=0 ;i < strlen(buffer);i++){
		printf("%c",buffer[i]);
	}
	printf("\n");
	if(send(cli_sock,buffer,sizeof(buffer),0)<0){
		perror("Send failed");
		close(cli_sock);
		close(serv_sock);
		exit(1);
	}
	close(cli_sock);
	close(serv_sock);
	return 0;
}
