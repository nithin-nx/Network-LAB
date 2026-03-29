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
	int len;
	cli_sock=socket(AF_INET,SOCK_DGRAM,0);
	if(cli_sock<0){
		perror("Socket creation failed");
		exit(1);
	}
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(5000);
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	printf("Enter the message to server:\n");
	if(scanf("%s",buffer)<=0){
		perror("Input failed");
		close(cli_sock);
		exit(1);
	}
	printf("Message %s is send to server",buffer);
	printf("\n");
	len=sizeof(serv_addr);
	if(sendto(cli_sock,buffer,sizeof(buffer),0,(struct sockaddr*)&serv_addr,len)<0){
		perror("Send failed");
		close(cli_sock);
		exit(1);
	}
	if(recvfrom(cli_sock,buffer,sizeof(buffer),0,NULL,NULL)<0){
		perror("Receive failed");
		close(cli_sock);
		exit(1);
	}
	close(cli_sock);
	return 0;
}
