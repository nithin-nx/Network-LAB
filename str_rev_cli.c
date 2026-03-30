#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() 
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[MAX];

    // Create a TCP socket (IPv4 + stream-based)
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Set server address details (IP + Port)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);                 // Convert port to network byte order
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Convert IP string to binary form

    // Establish connection to the server
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Send data to the server
    fgets(buffer, MAX, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    write(sock, buffer, strlen(buffer));

    // Receive response from the server
    memset(buffer, 0, MAX);
    read(sock, buffer, MAX);

    // Display server response
    printf("%s\n", buffer);

    // Close the socket connection
    close(sock);

    return 0;
}
