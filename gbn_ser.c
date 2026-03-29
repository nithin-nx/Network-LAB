#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
int main(){
	int servsocket,clisocket;
	int n,frame,ack;
	struct sockaddr_in servaddr;
	servsocket=socket(AF_INET,SOCK_STREAM,0);
	if(servsocket < 0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(9000);
	servaddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(servsocket,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		perror("Bind Failed");
		close(servsocket);
		exit(1);
	}
	if(listen(servsocket,5)<0){
		perror("Listen failed");
		close(servsocket);
		exit(1);
	}
	printf("Server is waiting.....\n");
	clisocket=accept(servsocket,NULL,NULL);
	if(clisocket < 0){
		perror("Accept failed");
		close(servsocket);
		exit(1);
	}
	while(1){
		if(recv(clisocket,&frame,sizeof(frame),0) < 0){
			perror("Receive failed");
			break;
		}
		printf("Frame %d received\n",frame);
		printf("Sending ACK %d\n",frame); 
		ack=frame;
		if(send(clisocket,&ack,sizeof(ack),0) < 0){
			perror("Send failed");
			break;
		}
	}
	close(clisocket);
	close(servsocket);
	return 0;
}
