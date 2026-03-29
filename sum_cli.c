#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#define port 5000
int main(){
	int clisocket;
	struct sockaddr_in servaddr,cliaddr;
	int num[2],i,sum=0;
	socklen_t len;
	clisocket=socket(AF_INET,SOCK_DGRAM,0);
	if(clisocket<0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	printf("Enter the first number and second number:\n");
	for(i=0;i<2;i++){
		if(scanf("%d",&num[i])<=0){
			perror("Input failed");
			close(clisocket);
			exit(1);
		}
	}
	len=sizeof(servaddr);
	if(sendto(clisocket,num,sizeof(num),0,(struct sockaddr*)&servaddr,len)<0){
		perror("Send failed");
		close(clisocket);
		exit(1);
	}
	if(recvfrom(clisocket,&sum,sizeof(sum),0,(struct sockaddr*)&servaddr,&len)<0){
		perror("Receive failed");
		close(clisocket);
		exit(1);
	}
	printf("The sum is:%d",sum);
	printf("\n");
	printf("\n");
	close(clisocket);
	return 0;
}
