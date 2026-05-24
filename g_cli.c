#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080
#define WINDOW_SIZE 4

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <total_frames>\n", argv[0]);
        return 1;
    }

    int total_frames = atoi(argv[1]);

    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Set timeout for receiving ACK
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int base = 0;
    int next_seq = 0;

    while (base < total_frames) {

        // Send frames in window
        while (next_seq < base + WINDOW_SIZE && next_seq < total_frames) {
            printf("Sending Frame: %d\n", next_seq);

            sendto(sockfd, &next_seq, sizeof(next_seq), 0,
                   (struct sockaddr*)&server_addr, addr_len);

            next_seq++;
        }

        int ack;

        // Receive ACK from server
        int n = recvfrom(sockfd, &ack, sizeof(ack), 0, NULL, NULL);

        if (n < 0) {
            printf("Timeout! Resending from Frame %d\n", base);

            next_seq = base;  // Go-Back-N retransmission
            continue;
        }

        printf("Received ACK: %d\n", ack);
        base = ack;   // Move window (cumulative ACK)
    }

    close(sockfd);  // Close socket
    return 0;
}
