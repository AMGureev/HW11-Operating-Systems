#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 256
#define MULTICAST_IP "239.0.0.1"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <server port>\n", argv[0]);
        exit(1);
    }
    
    int client_fd;
    struct sockaddr_in client_addr;
    struct ip_mreq mreq;
    char buffer[BUF_SIZE] = {0};
    socklen_t addr_len = sizeof(client_addr);

    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(1);
    }
    
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(atoi(argv[1]));

    if (bind(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind failed");
        exit(1);
    }

    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(client_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt failed");
        exit(1);
    }

    printf("Listening for multicast messages on IP: %s, port: %d\n", MULTICAST_IP, ntohs(client_addr.sin_port));

    while (1) {
        int len = recvfrom(client_fd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        buffer[len] = '\0';
        printf("Received message: %s\n", buffer);
        
        memset(buffer, 0, BUF_SIZE);
    }

    close(client_fd);
    return 0;
}
