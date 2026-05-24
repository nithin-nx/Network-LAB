#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main()
{
    int s,n;
    char b[1000],m[1000];
    struct sockaddr_in a;
    socklen_t l;

    s=socket(AF_INET,SOCK_DGRAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=INADDR_ANY;

    printf("Connected to server\n");

    fgets(m,1000,stdin);
	l= sizeof(a);
    sendto(s,m,strlen(m),0,(struct sockaddr*)&a,l);

    n=recvfrom(s,b,1000,0,(struct sockaddr*)&a,&l);
    b[n]=0;
    printf("Echo: %s",b);

    close(s);
}
