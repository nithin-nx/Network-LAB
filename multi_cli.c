#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int s,n;
    char b[1000];

    struct sockaddr_in a;
    socklen_t l;

    s=socket(AF_INET,SOCK_DGRAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(9090);
    a.sin_addr.s_addr=inet_addr("127.0.0.1");

    l=sizeof(a);

    while(1)
    {
        printf("Message: ");
        fgets(b,1000,stdin);

        if(strncmp(b,"exit",4)==0)
            break;

        sendto(s,b,strlen(b),0,
              (struct sockaddr*)&a,l);

        n=recvfrom(s,b,1000,0,NULL,NULL);

        b[n]=0;

        printf("Reply: %s",b);
    }

    close(s);
}
