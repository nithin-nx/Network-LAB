#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 100
#define PORT 8080

int is_in_list(int frame, int list[], int size, int *index) {
    for (int i = 0; i < size; i++) {
        if (list[i] == frame) {
            *index = i;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <total_frames>\n", argv[0]);
        return 1;
    }

    int total_frames = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int lost_list[MAX], timeout_list[MAX];
    int num_lost, num_timeout;

    // Create a UDP socket (IPv4 + datagram-based communication)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    // Configure server address (IP + Port)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);        // Convert port to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept data from any interface

    // Bind socket to the specified port (required for receiving data)
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    printf("Enter number of lost frames: ");
    scanf("%d", &num_lost);
    for (int i = 0; i < num_lost; i++)
        scanf("%d", &lost_list[i]);

    printf("Enter number of timeout frames: ");
    scanf("%d", &num_timeout);
    for (int i = 0; i < num_timeout; i++)
        scanf("%d", &timeout_list[i]);

    int next_frame = 0;

    while (next_frame < total_frames) {
        int recv_frame;

        // Receive a frame from client (UDP is connectionless)
        int n = recvfrom(sockfd, &recv_frame, sizeof(recv_frame), 0,
                         (struct sockaddr*)&client_addr, &addr_len);

        if (n < 0) {
            perror("recvfrom error");
            continue;
        }

        printf("Received Frame: %d\n", recv_frame);

        int index;

        if (is_in_list(recv_frame, lost_list, num_lost, &index)) {
            int nak = -1;

            // Send NAK (Negative Acknowledgement) to client
            sendto(sockfd, &nak, sizeof(nak), 0,
                   (struct sockaddr*)&client_addr, addr_len);

            printf("Sent NAK for Frame %d\n", recv_frame);
            lost_list[index] = -999;

        } else if (is_in_list(recv_frame, timeout_list, num_timeout, &index)) {
            // Simulate timeout (no response sent)
            printf("Simulating Timeout for Frame %d\n", recv_frame);
            timeout_list[index] = -999;

        } else {
            next_frame = recv_frame + 1;

            // Send ACK (Acknowledgement) with next expected frame
            sendto(sockfd, &next_frame, sizeof(next_frame), 0,
                   (struct sockaddr*)&client_addr, addr_len);

            printf("Sent ACK: %d\n", next_frame);
        }
    }

    // Close UDP socket
    close(sockfd);

    return 0;
}
