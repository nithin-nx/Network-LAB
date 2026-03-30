#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>

#define PORT 5000
#define MAXLINE 1024

int main() 
{
    int server_fd, client_fd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cli_len;
    char buffer[MAXLINE];
    int n;

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
        perror("Socket creation failed");
        exit(1);
    }

    // Clear server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;  // htonl(INADDR_ANY) also works
    servaddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
    {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) 
    {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("TCP Echo Server running on port %d...\n", PORT);

    cli_len = sizeof(cliaddr);

    // Accept a client connection
    client_fd = accept(server_fd, (struct sockaddr *)&cliaddr, &cli_len);
    if (client_fd < 0) 
    {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    printf("Client connected!\n");

    // Echo loop
    while (1) 
    {
        n = read(client_fd, buffer, sizeof(buffer));
        if (n <= 0) 
        {
            printf("Client disconnected.\n");
            break;
        }

        buffer[n] = '\0';  // Ensure null-terminated string
        printf("Received: %s\n", buffer);

        // Echo back to client
        if (write(client_fd, buffer, n) != n) 
        {
            perror("Write failed");
            break;
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
