#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int s,n;
    char b[1000];
    struct sockaddr_in a,c;
    socklen_t l;

    s=socket(AF_INET,SOCK_DGRAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=INADDR_ANY;

    bind(s,(struct sockaddr*)&a,sizeof(a));

    printf("Client waiting...\n");

    l=sizeof(c);

    n=recvfrom(s,b,1000,0,(struct sockaddr*)&c,&l);

    printf("Client connected\n");

    b[n]=0;

    printf("Message: %s",b);

    sendto(s,b,n,0,(struct sockaddr*)&c,l);

    printf("Client disconnected\n");

    close(s);
}
