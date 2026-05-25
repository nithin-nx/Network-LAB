#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main()
{
	int s,n;
	char b[100];
	struct sockaddr_in a;
	socklen_t l;
	
	s=socket(AF_INET,SOCK_DGRAM,0);
	
	a.sin_family=AF_INET;
	a.sin_port = htons(5000);
	a.sin_addr.s_addr = INADDR_ANY;
	
	bind(s,(struct sockaddr*)&a,sizeof(a));
	
	l=sizeof(a);
	
	n=recvfrom(s,b,100,0,(struct sockaddr*)&a,&l);
	b[n]=0;
	int len =strlen(b);
	for(int i = 0;i <len/2;i++)
	{
		char temp = b[i];
		b[i]=b[len-i-1];
		b[len-i-1]=temp;
	}
	sendto(s,b,100,0,(struct sockaddr*)&a,l);
	
	close(s);
	return 0;
}		
