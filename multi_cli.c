#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#define PORT 9090
#define SIZE 1024
int main() {
    int sockfd;
    struct sockaddr_in server_addr = {0};
    char buffer[SIZE];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1",&server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Enter messages:\n");
    while (1) {
        if (fgets(buffer, SIZE, stdin) == NULL) {
            perror("fgets failed");
            break;
        }
        if (strncmp(buffer, "exit", 4) == 0)
            break;
        ssize_t sent = sendto(sockfd, buffer, strlen(buffer), 0,(struct sockaddr*)&server_addr,sizeof(server_addr));
        if (sent == -1) {
            perror("sendto failed");
            continue;
        }
        ssize_t len = recvfrom(sockfd, buffer, SIZE - 1,0, NULL, NULL);
        if (len == -1) {
            perror("recvfrom failed");
            continue;
        }
        buffer[len] = '\0';
        printf("Server reply: %s", buffer);
    }
    
    close(sockfd);
    return 0;
}
