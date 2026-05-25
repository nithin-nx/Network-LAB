#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main()
{
	int s,n,m[2],sum,c;
	struct sockaddr_in a;
	
	s=socket(AF_INET,SOCK_STREAM,0);
	
	a.sin_family = AF_INET;
	a.sin_port = htons(5000);
	a.sin_addr.s_addr= inet_addr("127.0.0.1");
	
	connect(s,(struct sockaddr*)&a,sizeof(a));
	
	
		printf("ENTER NUMBERS\n");
		scanf("%d %d",&m[0],&m[1]);
		write(s,m,sizeof(m));
		
		read(s,&sum,sizeof(sum));
		printf("sum is : %d\n",sum);
	
	close(s);
	return 0;
}		
