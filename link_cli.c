#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define m 20
int main(){
	int n,start,cost[m][m],dist[m],i,j;
	int clisocket;
	struct sockaddr_in servaddr;
	clisocket=socket(AF_INET,SOCK_STREAM,0);
	if(clisocket<0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(9000);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(connect(clisocket,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		perror("Connection failed");
		close(clisocket);
		exit(1);
	}
	printf("Enter the number of nodes:\n");
	if(scanf("%d",&n)<=0){
		perror("Input failed");
		close(clisocket);
		exit(1);
	}
	printf("Enter the cost matrix:\n");
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(scanf("%d",&cost[i][j])<=0){
				perror("Input failed");
				close(clisocket);
				exit(1);
			}
		}
	}
	printf("Enter the source node:\n");
	if(scanf("%d",&start)<=0){
		perror("Input failed");
		close(clisocket);
		exit(1);
	}
	if(send(clisocket,&n,sizeof(n),0)<0){
		perror("Send failed");
		close(clisocket);
		exit(1);
	}
	if(send(clisocket,&start,sizeof(start),0)<0){
		perror("Send failed");
		close(clisocket);
		exit(1);
	}
	if(send(clisocket,cost,sizeof(cost),0)<0){
		perror("Send failed");
		close(clisocket);
		exit(1);
	}
	if(recv(clisocket,dist,sizeof(dist),0)<0){
		perror("Receive failed");
		close(clisocket);
		exit(1);
	}
	printf("Shortest distance\n");
	int sum=0;
	for(i=0;i<n;i++){
		printf("To node %d =%d\n",i,dist[i]);
		sum+=dist[i];
	}
	printf("Total cost is %d\n",sum);
	close(clisocket);
    return 0;
}
