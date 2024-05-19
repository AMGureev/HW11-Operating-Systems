#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Arguments: %s <server IP> <server port>\n", argv[0]);
        exit(1);
    }
    
    int client_fd = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE] = {0};

    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket failed \n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("inet_pton failed\n");
        exit(1);
    }

    printf("Connected to the server\n");

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);
        sendto(client_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        if (strcmp(buffer, "The End\n") == 0) {
            printf("The end\n");
            break;
        }
    }

    close(client_fd);
    return 0;
}
