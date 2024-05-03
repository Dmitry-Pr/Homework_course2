#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXPENDING 5
#define BUFFSIZE 32

void Die(const char *mess) {
    perror(mess);
    exit(1);
}

int main(int argc, char *argv[]) {
    int serversock, clientsock1, clientsock2;
    struct sockaddr_in echoserver, echoclient;
    char buffer[BUFFSIZE];
    unsigned int clientlen;
    int received = 0;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server IP> <Server Port>\n",
                argv[0]);
        exit(1);
    }

    if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        Die("Failed to create socket");
    }

    memset(&echoserver, 0, sizeof(echoserver));
    echoserver.sin_family = AF_INET;
    echoserver.sin_addr.s_addr = inet_addr(argv[1]);
    echoserver.sin_port = htons(atoi(argv[2]));

    if (bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
        Die("Failed to bind the server socket");
    } else {
        fprintf(stdout, "Server socket binded\n");
    }

    if (listen(serversock, MAXPENDING) < 0) {
        Die("Failed to listen on server socket");
    }
    printf("Server IP address = %s. Wait...\n", inet_ntoa(echoserver.sin_addr));
    while (1) {
        clientlen = sizeof(echoclient);

        if ((clientsock1 = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
            Die("Failed to accept client connection");
        } else {
            fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoclient.sin_addr));
        }

        if ((clientsock2 = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
            Die("Failed to accept client connection");
        } else {
            fprintf(stdout, "Client2 connected: %s\n", inet_ntoa(echoclient.sin_addr));
        }

        if ((received = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
            Die("Failed to receive initial bytes from client");
        }
        buffer[received] = '\0';
        if (strcmp(buffer, "The End\n") == 0) {
            break;
        }
        bool flag = false;
        while (received > 0) {
            if (send(clientsock2, buffer, received, 0) != received) {
                Die("Failed to send bytes to client");
            } else {
                fprintf(stdout, "Sending a message\n");
            }
            if (strcmp(buffer, "The End\n") == 0) {
                flag = true;
                break;
            }
            if ((received = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
                Die("Failed to receive additional bytes from client");
            }
            buffer[received] = '\0';

        }
        if (flag) {
            break;
        }

    }
    close(clientsock1);
    close(clientsock2);
    return 0;
}