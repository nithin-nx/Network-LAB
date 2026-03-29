#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
int main(){
	int clisocket;
	struct sockaddr_in servaddr;
	char buffer[200],newbuffer[200],filename[20];
	FILE *fp;
	clisocket=socket(AF_INET,SOCK_STREAM,0);
	if(clisocket<0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_port=htons(8080);
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(connect(clisocket,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		perror("Connection failed");
		close(clisocket);
		exit(1);
	}
	printf("Enter the command\n");
	if(scanf("%s",buffer)<=0){
		perror("Input failed");
		close(clisocket);
		exit(1);
	}
	printf("Enter the filename\n");
	if(scanf("%s",filename)<=0){
		perror("Input failed");
		close(clisocket);
		exit(1);
	}
	if(send(clisocket,buffer,sizeof(buffer),0)<0){
		perror("Send failed");
		close(clisocket);
		exit(1);
	}
	if(send(clisocket,filename,sizeof(filename),0)<0){
		perror("Send failed");
		close(clisocket);
		exit(1);
	}
	if(strcmp(buffer,"GET")==0){
		if(recv(clisocket,newbuffer,sizeof(newbuffer),0)<0){
			perror("Receive failed");
			close(clisocket);
			exit(1);
		}
		if(strcmp(newbuffer,"ERROR")==0){
    			printf("File not found on server\n");
		}
		else{
			fp=fopen(filename,"w");
			fputs(newbuffer,fp);
			fclose(fp);
			printf("File downloaded\n");
			printf("\n");
		}
	}
	else if(strcmp(buffer,"PUT")==0){
		fp=fopen(filename,"r");
		if(fp==NULL){
			strcpy(newbuffer,"ERROR");
			send(clisocket,newbuffer,sizeof(newbuffer),0);
			close(clisocket);
			exit(1);
		}
		fgets(newbuffer,sizeof(newbuffer),fp);
		if(send(clisocket,newbuffer,sizeof(newbuffer),0)<0){
			perror("Send failed");
			fclose(fp);
			close(clisocket);
			exit(1);
		}
		fclose(fp);
		printf("File uploaded\n");
		printf("\n");
	}
	close(clisocket);
    return 0;
}
