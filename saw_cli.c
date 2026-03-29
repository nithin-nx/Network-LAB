#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
int main(){
	int clisocket;
	struct sockaddr_in servaddr;
	int frame=0,ack;
	clisocket=socket(AF_INET,SOCK_STREAM,0);
	if(clisocket < 0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(9000);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(connect(clisocket,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0){
		perror("Connection failed");
		close(clisocket);
		exit(1);
	}
	srand(time(0));
	int n;
	printf("Max num of frame to be sent:\n");

	if(scanf("%d",&n) <= 0){
		perror("Input failed");
		close(clisocket);
		exit(1);
	}
	while(n>0){
		if(rand()%5==0){
			printf("Frame is lost \n");
			printf("Retransmitting frame %d\n",frame);
		}
		else{
			printf("Sending Frame %d\n",frame);

			if(send(clisocket,&frame,sizeof(frame),0) < 0){
				perror("Send failed");
				break;
			}
			if(recv(clisocket,&ack,sizeof(ack),0) < 0){
				perror("Receive failed");
				break;
			}
			printf("Received ACK %d\n",ack);

			if(ack==(frame+1)%2){
				frame=ack;
				n--;
			}
			else{
				printf("ACK error, retransmitting frame %d\n",frame);
			}
		}
	}
	close(clisocket);
	return 0;
}
