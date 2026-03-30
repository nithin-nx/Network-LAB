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
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    int n;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        perror("Socket creation failed");
        return 1;
    }

    // Clear addresses
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
    {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    printf("UDP Echo Server running on port %d...\n", PORT);

    len = sizeof(cliaddr);

    // Receive message
    n = recvfrom(sockfd, buffer, MAXLINE, 0,(struct sockaddr *)&cliaddr, &len);

    if (n < 0) 
    {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Received: %s\n", buffer);

    // Echo back the same message
    sendto(sockfd, buffer, n, 0,
           (struct sockaddr *)&cliaddr, len);

    close(sockfd);
    return 0;
}
