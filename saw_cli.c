#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 8080

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <total_frames>\n", argv[0]);
        return 1;
    }

    int total_frames = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    // Create a UDP socket (connectionless communication)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    // Configure server address (IP + Port)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);                 // Convert port to network byte order
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Convert IP to binary form

    // Set receive timeout for socket (important for retransmission)
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int next_frame = 0;

    while (next_frame < total_frames) {
        printf("Sending Frame: %d\n", next_frame);

        // Send frame to server (no connection required in UDP)
        sendto(sockfd, &next_frame, sizeof(next_frame), 0,
               (struct sockaddr*)&server_addr, addr_len);

        int ack_frame;

        // Receive ACK/NAK from server
        int n = recvfrom(sockfd, &ack_frame, sizeof(ack_frame), 0, NULL, NULL);

        if (n < 0) {
            // Timeout occurred (no response received)
            printf("Timeout! Resending Frame %d\n", next_frame);
            continue;
        }

        if (ack_frame == -1) {
            // NAK received → resend same frame
            printf("Received NAK for Frame %d\n", next_frame);
            continue;
        }

        if (ack_frame == next_frame + 1) {
            // ACK received → move to next frame
            printf("Received ACK: %d\n", ack_frame);
            next_frame++;
        }
    }

    // Close UDP socket
    close(sockfd);

    return 0;
}
