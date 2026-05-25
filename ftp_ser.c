#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{
    int s,c,n;

    char b[1024],cmd[20],f[100];

    FILE *fp;

    struct sockaddr_in a;

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(5000);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&a, sizeof(a));

    listen(s, 5);

    while(1)
    {
        c = accept(s, NULL, NULL);

        n = read(c, b, 1024);
        b[n] = 0;
        sscanf(b, "%s %s", cmd, f);

        if(strcmp(cmd, "get") == 0)
        {
            fp = fopen(f, "rb");
            while((n = fread(b, 1, 1024, fp)) > 0)
            {
                write(c, b, n);
            }
            fclose(fp);
            printf("Sent\n");
        }

        else
        {
            fp = fopen(f, "wb");
            while((n = read(c, b, 1024)) > 0)
            {
                fwrite(b, 1, n, fp);
            }
            fclose(fp);
            printf("Received\n");
        }
        close(c);
    }
    close(s);
    
    return 0;
}
