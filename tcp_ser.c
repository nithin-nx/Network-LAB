#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int s,c,n;
    char b[1024];
    struct sockaddr_in a;

    s=socket(AF_INET,SOCK_STREAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=INADDR_ANY;

    bind(s,(struct sockaddr*)&a,sizeof(a));
    listen(s,5);

    c=accept(s,NULL,NULL);
	printf("Client connected\n");

    while((n=read(c,b,1024))>0)
    {
        b[n]=0;
        printf("Received: %s\n",b);
        write(c,b,n);
    }
	printf("client disconnected\n");
    close(c);
    close(s);
}
