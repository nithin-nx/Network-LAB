#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, port, n;
    socklen_t clilen;
    char buffer[BUF_SIZE], command[10], filename[256];
    struct sockaddr_in serv_addr, cli_addr;
    FILE *fp;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Socket error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    port = atoi(argv[1]);

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Bind failed");

    // Listen for incoming connections
    listen(sockfd, 5);
    printf("FTP Server started...\n");

    clilen = sizeof(cli_addr);

    while (1) {
        // Accept client connection
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) error("Accept failed");

        printf("Client connected\n");

        memset(buffer, 0, BUF_SIZE);

        // Receive command from client
        read(newsockfd, buffer, BUF_SIZE);

        sscanf(buffer, "%s %s", command, filename);

        // GET
        if (strcmp(command, "get") == 0) {
            fp = fopen(filename, "rb");

            if (!fp) {
                write(newsockfd, "ERROR\n", 6);
            } else {
                write(newsockfd, "OK\n", 3);

                while ((n = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
                    write(newsockfd, buffer, n);  // Send file data
                }

                fclose(fp);
                printf("File sent: %s\n", filename);
            }
        }

        // PUT
        else if (strcmp(command, "put") == 0) {
            fp = fopen(filename, "wb");

            if (!fp) {
                write(newsockfd, "ERROR\n", 6);
            } else {
                write(newsockfd, "OK\n", 3);

                while ((n = read(newsockfd, buffer, BUF_SIZE)) > 0) {
                    fwrite(buffer, 1, n, fp);  // Receive file data
                }

                fclose(fp);
                printf("File received: %s\n", filename);
            }
        }

        close(newsockfd);  // Close client socket
    }

    close(sockfd);  // Close server socket
    return 0;
}
