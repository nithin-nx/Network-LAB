#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int nums[2], sum;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;   // Accept from any IP
    server_addr.sin_port = htons(PORT);         // Port in network byte order

    // Bind socket to port
    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Server ready on port %d\n", PORT);

    // Receive data from client
    recvfrom(sock, nums, sizeof(nums), 0,
             (struct sockaddr *)&client_addr, &addr_len);

    // Process data
    sum = nums[0] + nums[1];

    // Send result back to client
    sendto(sock, &sum, sizeof(sum), 0,
           (struct sockaddr *)&client_addr, addr_len);

    // Close socket
    close(sock);

    return 0;
}
