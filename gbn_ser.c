#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int s,f,ack;

    struct sockaddr_in a;
    socklen_t l;

    s=socket(AF_INET,SOCK_DGRAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=INADDR_ANY;

    bind(s,(struct sockaddr*)&a,sizeof(a));

    l=sizeof(a);

    while(1)
    {
        recvfrom(s,&f,sizeof(f),0,
                (struct sockaddr*)&a,&l);

        if(f==-1)
            break;

        printf("Frame %d received\n",f);

        ack=f+1;

        sendto(s,&ack,sizeof(ack),0,
              (struct sockaddr*)&a,l);

        printf("ACK %d sent\n",ack);
    }

    close(s);
}
