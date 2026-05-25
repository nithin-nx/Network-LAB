#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{
    int s,n;
    char cmd[20],f[100],b[1024];
    FILE *fp;
    struct sockaddr_in a;

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(5000);
    a.sin_addr.s_addr = INADDR_ANY;

    connect(s, (struct sockaddr*)&a, sizeof(a));

    printf("Command(get/put): ");
    scanf("%s", cmd);

    printf("File: ");
    scanf("%s", f);

    sprintf(b, "%s %s", cmd, f);

    write(s, b, strlen(b));

    if(strcmp(cmd, "get") == 0)
    {
        fp = fopen(f, "wb");
        while((n = read(s, b, 1024)) > 0)
        {
            fwrite(b, 1, n, fp);
        }
        fclose(fp);
        printf("Downloaded\n");
    }

    else
    {
        fp = fopen(f, "rb");
        while((n = fread(b, 1, 1024, fp)) > 0)
        {
            write(s, b, n);
        }
        fclose(fp);
        printf("Uploaded\n");
    }

    close(s);

    return 0;
}
