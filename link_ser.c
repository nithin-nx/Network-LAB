#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define max 999
#define MAX 20
int main(){
	int clisocket,servsocket;
	struct sockaddr_in servaddr;
	int i,j,n,start,min,next,cost[MAX][MAX];
	int visited[MAX],dist[MAX];
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
	printf("Server is waiting for connection...\n");
	clisocket=accept(servsocket,NULL,NULL);
	if(clisocket<0){
		perror("Accept failed");
		close(servsocket);
		exit(1);
	}
	if(recv(clisocket,&n,sizeof(n),0)<0){
		perror("Receive failed");
		close(clisocket);
		close(servsocket);
		exit(1);
	}
	if(recv(clisocket,&start,sizeof(start),0)<0){
		perror("Receive failed");
		close(clisocket);
		close(servsocket);
		exit(1);
	}
	if(recv(clisocket,cost,sizeof(cost),0)<0){
		perror("Receive failed");
		close(clisocket);
		close(servsocket);
		exit(1);
	}
	for(i=0;i<n;i++){
		dist[i]=cost[start][i];
		visited[i]=0;
	}
	dist[start]=0;
	visited[start]=1;
	for(i=1;i<n;i++){
		min=max;
		for(j=0;j<n;j++){
			if(dist[j]<min && visited[j]==0){
				min=dist[j];
				next=j;
			}
		}
		visited[next]=1;
		for(j=0;j<n;j++){
			if(visited[j]==0){
				if(min+cost[next][j]<dist[j]){
					dist[j]=min+cost[next][j];
				}
			}
		}
	}
	if(send(clisocket,dist,sizeof(dist),0)<0){
		perror("Send failed");
		close(clisocket);
		close(servsocket);
		exit(1);
	}
	close(clisocket);
	close(servsocket);
	return 0;
}
