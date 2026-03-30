#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[MAX];

    // Create a TCP socket (IPv4 + stream-oriented communication)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    // Configure server address (IP + Port)
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available network interfaces
    server_addr.sin_port = htons(PORT);       // Convert port to network byte order

    // Bind socket to the specified IP and port
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Put the socket into listening mode to accept incoming connections
    listen(server_fd, 5);

    // Accept a client connection (creates a new socket for communication)
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    // Display client's IP address and port
    printf("Client connected from %s:%d\n",
           inet_ntoa(client_addr.sin_addr), // Convert IP to readable format
           ntohs(client_addr.sin_port));    // Convert port to host byte order

    // Receive data from the client
    int n = read(client_fd, buffer, MAX - 1);
    buffer[n] = '\0';

    printf("Client message: %s\n", buffer);

    int len = strlen(buffer);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = buffer[i];
        buffer[i] = buffer[len - i - 1];
        buffer[len - i - 1] = temp;
    }

    // Send processed data back to the client
    write(client_fd, buffer, strlen(buffer));

    // Close client connection socket
    close(client_fd);

    // Close server socket
    close(server_fd);

    return 0;
}
