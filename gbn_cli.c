#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>

#define W 4

int main()
{
    int s,total,base=0,next=0,ack;

    struct sockaddr_in a;
    socklen_t l;

    s=socket(AF_INET,SOCK_DGRAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=inet_addr("127.0.0.1");

    printf("Enter total frames: ");
    scanf("%d",&total);

    l=sizeof(a);

    while(base<total)
    {
        while(next<base+W && next<total)
        {
            printf("Sending %d\n",next);

            sendto(s,&next,sizeof(next),0,
                  (struct sockaddr*)&a,l);

            recvfrom(s,&ack,sizeof(ack),0,NULL,NULL);

            printf("ACK %d received\n",ack);

            base=ack;

            next++;
        }
    }

    next=-1;

    sendto(s,&next,sizeof(next),0,
          (struct sockaddr*)&a,l);

    close(s);
}
