#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int s,n;
    char b[1024],m[1024];
    struct sockaddr_in a;

    s=socket(AF_INET,SOCK_STREAM,0);

    a.sin_family=AF_INET;
    a.sin_port=htons(5000);
    a.sin_addr.s_addr=inet_addr("127.0.0.1");

    connect(s,(struct sockaddr*)&a,sizeof(a));
	
	printf("Server connected\n");

    while(1)
    {
        fgets(m,1024,stdin);

        write(s,m,strlen(m));

        n=read(s,b,1024);
        b[n]=0;

        printf("Echo: %s",b);
    }
	
    close(s);
}
