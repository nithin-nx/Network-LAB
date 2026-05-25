#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main()
{
	int s,n;
	char b[100],m[100];
	struct sockaddr_in a;
	socklen_t l;
	
	s=socket(AF_INET,SOCK_DGRAM,0);
	
	a.sin_family = AF_INET;
	a.sin_port = htons(5000);
	a.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	fgets(m,100,stdin);
	l=sizeof(a);
	sendto(s,m,100,0,(struct sockaddr*)&a,l);
	
	n=recvfrom(s,b,100,0,(struct sockaddr*)&a,&l);
	b[n]=0;
	printf("REV string is %s",b);
	
	close(s);
	return 0;
}	
