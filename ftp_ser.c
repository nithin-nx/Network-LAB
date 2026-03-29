#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
int main(){
	FILE *fp;
	char buffer[200],filename[20],newbuffer[200];
	int clisocket,servsocket;
	struct sockaddr_in servaddr;
	servsocket=socket(AF_INET,SOCK_STREAM,0);
	if(servsocket<0){
		perror("Socket creation failed");
		exit(1);
	}
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(8080);
	servaddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(servsocket,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		perror("Bind failed");
		close(servsocket);
		exit(1);
	}
	if(listen(servsocket,5)<0){
		perror("Listen failed");
		close(servsocket);
		exit(1);
	}
	printf("Server is waiting.......\n");
	clisocket=accept(servsocket,NULL,NULL);
	if(clisocket<0){
		perror("Accept failed");
		close(servsocket);
		exit(1);
	}
	if(recv(clisocket,buffer,sizeof(buffer),0)<0){
		perror("Receive failed");
		close(clisocket);
		close(servsocket);
		exit(1);
	}
	if(recv(clisocket,filename,sizeof(filename),0)<0){
		perror("Receive failed");
		close(clisocket);
		close(servsocket);
		exit(1);
	}
	if(strcmp(buffer,"GET")==0){
		fp=fopen(filename,"r");
		if(fp==NULL){
			strcpy(newbuffer,"ERROR");
    			send(clisocket,newbuffer,sizeof(newbuffer),0);
    			close(clisocket);
			close(servsocket);
    			exit(1);
		}
		while(fgets(newbuffer,sizeof(newbuffer),fp)){
			if(send(clisocket,newbuffer,strlen(newbuffer),0)<0){
				perror("Send failed");
				fclose(fp);
				close(clisocket);
				close(servsocket);
				exit(1);
			}
			printf("File sent successfully\n");
			printf("\n");
		}
		fclose(fp);
	}
	else if(strcmp(buffer,"PUT")==0){
		fp=fopen(filename,"w");
		if(fp==NULL){
			strcpy(newbuffer,"ERROR");
    			send(clisocket,newbuffer,sizeof(newbuffer),0);
    			close(clisocket);
			close(servsocket);
    			exit(1);
    		}
		if(recv(clisocket,newbuffer,sizeof(newbuffer),0)<0){
			perror("Receive failed");
			fclose(fp);
			close(clisocket);
			close(servsocket);
			exit(1);
		}
		fputs(newbuffer,fp);
		fclose(fp);
		printf("File received\n");
		printf("\n");
	}
	close(clisocket);
	close(servsocket);
    return 0;
}
