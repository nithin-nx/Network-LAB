#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr = {0};
    char buffer[SIZE];

    // Create UDP socket (connectionless communication)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address (IP + Port)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);  // Convert port to network byte order

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter messages:\n");

    while (1) {
        if (!fgets(buffer, SIZE, stdin))
            break;

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        // Send message to server (no connection needed in UDP)
        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr));

        // Receive response from server
        ssize_t len = recvfrom(sockfd, buffer, SIZE - 1, 0, NULL, NULL);

        if (len < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[len] = '\0';

        // Display server reply
        printf("Server reply: %s", buffer);
    }

    // Close UDP socket
    close(sockfd);

    return 0;
}
