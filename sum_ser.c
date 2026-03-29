#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#define port 5000
int main(){

	int servsocket;
	struct sockaddr_in servaddr,cliaddr;
	int num[3],sum=0;
	socklen_t len;
	servsocket=socket(AF_INET,SOCK_DGRAM,0);
	if(servsocket<0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(servsocket,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		perror("Bind failed");
		close(servsocket);
		exit(1);
	}
	len=sizeof(cliaddr);
	if(recvfrom(servsocket,num,sizeof(num),0,(struct sockaddr*)&cliaddr,&len)<0){
		perror("Receive failed");
		close(servsocket);
		exit(1);
	}
	printf("The two numbers from client:%d %d\n",num[0],num[1]);
	sum=num[0]+num[1];
	printf("The sum of numbers:%d\n",sum);
	if(sendto(servsocket,&sum,sizeof(sum),0,(struct sockaddr*)&cliaddr,len)<0){
		perror("Send failed");
		close(servsocket);
		exit(1);
	}
	printf("The sum of the numbers is sent successfully:\n");
	printf("\n");
	close(servsocket);
	return 0;

}
