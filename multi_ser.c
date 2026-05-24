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
    a.sin_port=htons(9090);
    a.sin_addr.s_addr=INADDR_ANY;

    bind(s,(struct sockaddr*)&a,sizeof(a));

    printf("Server started\n");

    l=sizeof(c);

    while(1)
    {
        n=recvfrom(s,b,1000,0,
                  (struct sockaddr*)&c,&l);

        b[n]=0;

        printf("Client: %s",b);

        sendto(s,b,n,0,
              (struct sockaddr*)&c,l);
    }

    close(s);
}
