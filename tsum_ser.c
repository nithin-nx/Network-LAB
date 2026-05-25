#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main()
{
	int s,n,c,sum,m[2];
	struct sockaddr_in a;
	
	s=socket(AF_INET,SOCK_STREAM,0);
	
	a.sin_family = AF_INET;
	a.sin_port = htons(5000);
	a.sin_addr.s_addr = INADDR_ANY;
	
	bind(s,(struct sockaddr*)&a,sizeof(a));
	listen(s,5);
	c=accept(s,NULL,NULL);
	
	while((n=read(c,m,sizeof(m)))>0)
	{
		printf("NUMBERS ARE %d %d\n",m[0],m[1]);
		sum= m[0]+m[1];
		write(c,&sum,sizeof(sum));
	}
	close(c);
	close(s);
	return 0;
	
}		
