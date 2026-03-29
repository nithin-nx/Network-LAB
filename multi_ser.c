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
    if (flags == -1) {
        perror("fcntl GETFL failed");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl SETFL failed");
        return -1;
    }
    return 0;
}
int main() {
    int sockfd, epfd;
    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};
    socklen_t client_len;
    char buffer[SIZE];
    struct epoll_event ev, events[MAX];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
    }
    if (set_nonblocking(sockfd) == -1) {
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1 failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        perror("epoll_ctl failed");
        close(sockfd);
        close(epfd);
        exit(EXIT_FAILURE);
    }
    printf("UDP Server running on port %d\n", PORT);
    while (1) {
        int nfds = epoll_wait(epfd, events, MAX, -1);
        if (nfds == -1) {
            perror("epoll_wait failed");
            break;
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == sockfd) {
                while (1) {
                    client_len = sizeof(client_addr);
                    ssize_t len = recvfrom(sockfd, buffer, SIZE - 1, 0,(struct sockaddr*)&client_addr,&client_len);
                    if (len == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break; 
                        perror("recvfrom failed");
                        break;
                    }
                    buffer[len] = '\0';
                    printf("Client %s:%d → %s\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port), buffer);
                    if (sendto(sockfd, buffer, len, 0,(struct sockaddr*)&client_addr, client_len) == -1) {
                        perror("sendto failed");
                    }
                }
            }
        }
    }
    close(sockfd);
    close(epfd);
    return 0;
}
