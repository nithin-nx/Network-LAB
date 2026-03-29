#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#define max 999
#define MAX 20
int main(){
	int clisocket,servsocket;
	struct sockaddr_in servaddr;
	int frame,ack;
	servsocket=socket(AF_INET,SOCK_STREAM,0);
	if(servsocket < 0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(9000);
	servaddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(servsocket,(struct sockaddr*)&servaddr,sizeof(servaddr))< 0){
		perror("Bind failed");
		close(servsocket);
		exit(1);
	}
	if(listen(servsocket,5)< 0){
		perror("Listen failed");
		close(servsocket);
		exit(1);
	}
	srand(time(0));
	printf("Server is waiting for connection...\n");
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

		printf("Frame %d received \n",frame);
		if(rand()%5 == 0){
			printf("ACK is lost\n");
			ack=-1;
			printf("Retransmitting frame %d\n",frame);

			if(send(clisocket,&ack,sizeof(ack),0) < 0){
				perror("Send failed");
				break;
			}
		}
		else{
			ack=(frame+1)%2;
			printf("Sending ACK %d\n",ack);

			if(send(clisocket,&ack,sizeof(ack),0) < 0){
				perror("Send failed");
				break;
			}
		}
	}
	close(clisocket);
	close(servsocket);
	return 0;
}
