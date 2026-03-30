#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

#define PORT 9090
#define MAX 10
#define SIZE 1024

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    // Set socket to non-blocking mode (required for epoll)
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int sockfd, epfd;
    struct sockaddr_in server_addr = {0}, client_addr;
    socklen_t client_len;
    char buffer[SIZE];
    struct epoll_event ev, events[MAX];

    // Create UDP socket (connectionless communication)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address (IP + Port)
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept data from any interface
    server_addr.sin_port = htons(PORT);       // Convert port to network byte order

    // Bind socket to port (required to receive data)
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Set socket to non-blocking for asynchronous I/O
    if (set_nonblocking(sockfd) == -1) {
        perror("non-blocking failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Create epoll instance (efficient I/O event notification)
    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1 failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Register socket with epoll to monitor incoming data
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    ev.data.fd = sockfd;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        perror("epoll_ctl failed");
        close(sockfd);
        close(epfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Multi-client Server running on port %d\n", PORT);

    while (1) {
        // Wait for events on the socket
        int nfds = epoll_wait(epfd, events, MAX, -1);
        if (nfds == -1) {
            perror("epoll_wait failed");
            break;
        }

        for (int i = 0; i < nfds; i++) {

            // Check for socket errors
            if (events[i].events & (EPOLLERR | EPOLLHUP)) {
                fprintf(stderr, "Epoll error\n");
                continue;
            }

            // If data is available on socket
            if (events[i].data.fd == sockfd) {
                while (1) {
                    client_len = sizeof(client_addr);

                    // Receive datagram from client
                    ssize_t len = recvfrom(sockfd, buffer, SIZE - 1, 0,
                        (struct sockaddr*)&client_addr, &client_len);

                    if (len == -1) {
                        // No more data (non-blocking mode)
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        perror("recvfrom failed");
                        break;
                    }

                    buffer[len] = '\0';

                    // Convert client IP to readable format
                    char ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));

                    printf("Client %s:%d → %s",
                           ip, ntohs(client_addr.sin_port), buffer);

                    // Send response back to client (echo)
                    if (sendto(sockfd, buffer, len, 0,
                               (struct sockaddr*)&client_addr, client_len) == -1) {
                        perror("sendto failed");
                    }
                }
            }
        }
    }

    // Close socket and epoll instance
    close(sockfd);
    close(epfd);

    return 0;
}
