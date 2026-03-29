#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#define maxlen 200
int main(){
	int cli_sock;
	char buffer[maxlen];
	struct sockaddr_in serv_addr;
	cli_sock=socket(AF_INET,SOCK_STREAM,0);
	if(cli_sock<0){
		perror("Socket creation failed");
		exit(1);
	}
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(5000);
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(connect(cli_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
		perror("Connection failed");
		close(cli_sock);
		exit(1);
	}
	printf("Enter the message to server:\n");
	if(scanf("%s",buffer)<=0){
		perror("Input failed");
		close(cli_sock);
		exit(1);
	}
	printf("\n");
	if(send(cli_sock,buffer,sizeof(buffer),0)<0){
		perror("Send failed");
		close(cli_sock);
		exit(1);
	}
	if(recv(cli_sock,buffer,sizeof(buffer),0)<0){
		perror("Receive failed");
		close(cli_sock);
		exit(1);
	}
	close(cli_sock);
	return 0;	
}
