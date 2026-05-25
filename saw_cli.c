#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>


int main()
{
    int s,f=0,total,ack,n;

    struct sockaddr_in a;

    socklen_t l;

    struct timeval t;

    printf("Enter total frames: ");
    scanf("%d",&total);

    s=socket(AF_INET,SOCK_DGRAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=inet_addr("127.0.0.1");

    t.tv_sec=2;
    t.tv_usec=0;

    setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,&t,sizeof(t));

    l=sizeof(a);

    while(f<total)
    {
        printf("Sending Frame: %d\n",f);

        sendto(s,&f,sizeof(f),0,(struct sockaddr*)&a,l);

        n=recvfrom(s,&ack,sizeof(ack),0,NULL,NULL);

        if(n<0)
        {
            printf("Timeout! Resending Frame %d\n",f);
            continue;
        }

        if(ack==-1)
        {
            printf("Received NAK for Frame %d\n",f);
            continue;
        }

        printf("Received ACK: %d\n",ack);

        f++;
    }

    close(s);

    return 0;
}
