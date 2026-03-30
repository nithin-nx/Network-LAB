#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define MAXLINE 1000

int main() 
{
    int sockfd;
    char buffer[MAXLINE];
    char message[MAXLINE];
    struct sockaddr_in servaddr;
    socklen_t len;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        perror("Socket creation failed");
        return 1;
    }

    // Clear server address
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    printf("Enter message: ");
    fgets(message, MAXLINE, stdin);

    // Send message to server
    sendto(sockfd, message, strlen(message), 0,(struct sockaddr *)&servaddr, sizeof(servaddr));

    // Receive echoed message
    len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, MAXLINE, 0,(struct sockaddr *)&servaddr, &len);

    if (n < 0) 
    {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Echo from server: %s", buffer);

    close(sockfd);
    return 0;
}
