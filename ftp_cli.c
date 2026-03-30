#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, port, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char command[10], filename[256], buffer[BUF_SIZE];
    FILE *fp;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[2]);

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Socket creation failed");

    // Get server IP from hostname
    server = gethostbyname(argv[1]);
    if (!server) error("No such host");

    // Configure server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");

    printf("Enter command (get/put): ");
    scanf("%9s", command);
    printf("Enter filename: ");
    scanf("%255s", filename);

    snprintf(buffer, BUF_SIZE, "%s %s", command, filename);

    // Send command to server
    write(sockfd, buffer, strlen(buffer));

    memset(buffer, 0, BUF_SIZE);

    // Receive server response
    read(sockfd, buffer, BUF_SIZE);

    if (strncmp(buffer, "ERROR", 5) == 0) {
        printf("Server error\n");
        close(sockfd);
        return 0;
    }

    // GET
    if (strcmp(command, "get") == 0) {
        fp = fopen(filename, "wb");
        if (!fp) error("File open failed");

        while ((n = read(sockfd, buffer, BUF_SIZE)) > 0) {
            fwrite(buffer, 1, n, fp);  // Receive file data
        }

        fclose(fp);
        printf("Download successful\n");
    }

    // PUT
    else if (strcmp(command, "put") == 0) {
        fp = fopen(filename, "rb");
        if (!fp) {
            printf("File not found\n");
            close(sockfd);
            return 0;
        }

        while ((n = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
            write(sockfd, buffer, n);  // Send file data
        }

        fclose(fp);
        printf("Upload successful\n");
    }

    close(sockfd);  // Close socket
    return 0;
}
