#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int s,n;
    char b[1000],m[1000];
    struct sockaddr_in a;
    socklen_t l;

    s=socket(AF_INET,SOCK_DGRAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=inet_addr("127.0.0.1");

    printf("Connected to server\n");

    printf("Message: ");
    fgets(m,1000,stdin);

    sendto(s,m,strlen(m),0,(struct sockaddr*)&a,sizeof(a));

    l=sizeof(a);

    n=recvfrom(s,b,1000,0,(struct sockaddr*)&a,&l);

    b[n]=0;

    printf("Echo: %s",b);

    close(s);
}
