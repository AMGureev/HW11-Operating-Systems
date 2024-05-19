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
    
    int server_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(1);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(MULTICAST_IP);
    server_addr.sin_port = htons(atoi(argv[1]));

    printf("Multicasting on IP: %s, port: %d\n", MULTICAST_IP, ntohs(server_addr.sin_port));

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);
        sendto(server_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        
        memset(buffer, 0, BUF_SIZE);
    }

    close(server_fd);
    return 0;
}
