#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 100

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <total_frames>\n", argv[0]);
        return 1;
    }

    int total_frames = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    int expected = 0;

    while (expected < total_frames) {
        int frame;

        // Receive frame from client
        recvfrom(sockfd, &frame, sizeof(frame), 0,
                 (struct sockaddr*)&client_addr, &addr_len);

        printf("Received Frame: %d\n", frame);

        if (frame == expected) {
            expected++;
        } else {
            printf("Out of order frame! Expected %d\n", expected);
        }

        // Send cumulative ACK
        sendto(sockfd, &expected, sizeof(expected), 0,
               (struct sockaddr*)&client_addr, addr_len);

        printf("Sent ACK: %d\n", expected);
    }

    close(sockfd);  // Close socket
    return 0;
}
