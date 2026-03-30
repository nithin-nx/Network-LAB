#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    int nums[2], sum;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Configure server address (localhost)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter two numbers: ");
    scanf("%d %d", &nums[0], &nums[1]);

    // Send data to server
    sendto(sock, nums, sizeof(nums), 0,
           (struct sockaddr *)&server_addr, addr_len);

    printf("Message sent to server\n");

    // Receive result from server
    recvfrom(sock, &sum, sizeof(sum), 0,
             (struct sockaddr *)&server_addr, &addr_len);

    printf("Reply received from server\n");
    printf("Sum = %d\n", sum);

    // Close socket
    close(sock);

    return 0;
}
