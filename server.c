#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 256
char* ip = "127.0.0.1";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Arguments: %s <server port>\n", argv[0]);
        exit(1);
    }
    
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE] = {0};
    socklen_t addr_len = sizeof(client_addr);

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(1);
    }
    
    printf("Server's ip address: %s\n", inet_ntoa(server_addr.sin_addr));
    printf("Server's port: %d\n", ntohs(server_addr.sin_port));
    
    while (1) {
        int len = recvfrom(server_fd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        buffer[len] = '\0';
        printf("Received message: %s\n", buffer);
        
        if (strcmp(buffer, "The End\n") == 0) {
            printf("The end\n");
            break;
        }
    }

    close(server_fd);
    return 0;
}
