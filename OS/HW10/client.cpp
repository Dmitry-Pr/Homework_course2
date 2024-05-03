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
    unsigned int echolen;
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

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFSIZE, stdin);
        echolen = strlen(buffer);

        if (send(sock, buffer, echolen, 0) != echolen) {
            Die("Mismatch in number of sent bytes");
        }

        if (strcmp(buffer, "The End\n") == 0) {
            close(sock);
            break;
        }
    }

    return 0;
}