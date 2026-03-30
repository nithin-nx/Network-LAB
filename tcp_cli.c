#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define MAXLINE 1024

int main() 
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAXLINE];
    char message[MAXLINE];
    int n;

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("Socket creation failed");
        return 1;
    }

    // Clear server address
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
    {
        perror("Connect failed");
        return 1;
    }

    printf("Connected to server. Type messages to send:\n");

    while (1) 
    {
        printf("Enter message: ");
        fgets(message, MAXLINE, stdin);

        // Remove newline
        message[strcspn(message, "\n")] = 0;

        write(sockfd, message, strlen(message));

        n = read(sockfd, buffer, MAXLINE);
        if (n <= 0) 
        {
            printf("Server disconnected.\n");
            break;
        }

        buffer[n] = '\0';
        printf("Echo from server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
