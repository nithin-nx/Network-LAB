#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main()
{
    int s,n,m[2],sum;
    
    struct sockaddr_in a;
    socklen_t l;

    s=socket(AF_INET,SOCK_DGRAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=INADDR_ANY;

    printf("enter number");
    scanf("%d%d",&m[0],&m[1]);
    
	l= sizeof(a);
    sendto(s,m,sizeof(m),0,(struct sockaddr*)&a,l);

    recvfrom(s,&sum,sizeof(sum),0,(struct sockaddr*)&a,&l);
    
    printf("Echo: %d",sum);

    close(s);
}
