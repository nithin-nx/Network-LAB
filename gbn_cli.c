#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
int main()
{
    int clisocket;
    struct sockaddr_in servaddr;
    int n,i,ack,frame;
    srand(time(0));
    clisocket = socket(AF_INET,SOCK_STREAM,0);
    if(clisocket < 0){
        perror("Socket creation failed");
        exit(1);
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9000);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(clisocket,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0){
        perror("Connection failed");
        close(clisocket);
        exit(1);
    }
	printf("Enter the window size:\n");
	if(scanf("%d",&n) <= 0){
		perror("Input failed");
		close(clisocket);
		exit(1);
	}
	for(i=0;i<n;i++){
		printf("Sending frame %d\n",i);
		if(send(clisocket,&frame,sizeof(frame),0) < 0){
			perror("Send failed");
			break;
		}
		if(rand()%4==0){
			printf("ACK lost for frame %d\n",i);
			printf("Retransmitting frame %d\n",i);
			i--;
			continue;
		}
		if(recv(clisocket,&ack,sizeof(ack),0) < 0){
			perror("Receive failed");
			break;
		}
		printf("ACK %d received\n",ack);
	}
    close(clisocket);
    return 0;
}
