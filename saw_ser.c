#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int s,f,ack,total;
    int lost,time;
    struct sockaddr_in a,c;
    socklen_t l;

    printf("Enter total frames: ");
    scanf("%d",&total);

    printf("Enter lost frame: ");
    scanf("%d",&lost);

    printf("Enter timeout frame: ");
    scanf("%d",&time);

    s=socket(AF_INET,SOCK_DGRAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=INADDR_ANY;

    bind(s,(struct sockaddr*)&a,sizeof(a));

    l=sizeof(c);

    while(total)
    {
        recvfrom(s,&f,sizeof(f),0,(struct sockaddr*)&c,&l);

        printf("Received Frame: %d\n",f);

        if(f==lost)
        {
            ack=-1;

            sendto(s,&ack,sizeof(ack),0,(struct sockaddr*)&c,l);

            printf("Sent NAK for Frame %d\n",f);

            lost=-1;

            continue;
        }

        if(f==time)
        {
            printf("Simulating Timeout for Frame %d\n",f);

            time=-1;

            continue;
        }

        ack=f+1;

        sendto(s,&ack,sizeof(ack),0,(struct sockaddr*)&c,l);

        printf("Sent ACK: %d\n",ack);

        total--;
    }

    close(s);

    return 0;
}
