#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 32

void Die(const char *mess) {
    perror(mess);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in echoserver;
    char buffer[BUFFSIZE];
    int received = 0;

    if (argc != 3) {
        fprintf(stderr, "USAGE: client <server_ip> <port>\n");
        exit(1);
    }

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        Die("Failed to create socket");
    }

    memset(&echoserver, 0, sizeof(echoserver));
    echoserver.sin_family = AF_INET;
    echoserver.sin_addr.s_addr = inet_addr(argv[1]);
    echoserver.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
        Die("Failed to connect with server");
    }
    printf("Waiting for the server to send data...\n");
    while (1) {
        // Receive data from the server
        if ((received = recv(sock, buffer, BUFFSIZE - 1, 0)) <= 0) {
            Die("Failed to receive initial bytes from server");
        }

        // Null-terminate the received data
        buffer[received] = '\0';

        // Print out the received data
        printf("Received: %s", buffer);

        if (strcmp(buffer, "The End\n") == 0) {
            close(sock);
            break;
        }
    }

    return 0;
}