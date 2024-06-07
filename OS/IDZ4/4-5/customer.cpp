#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <arpa/inet.h>
#include "Message.cpp"

const int NUM_DEPARTMENTS = 3;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <PORT>" << std::endl;
        return 1;
    }

    srand(time(0));
    int sock = 0;
    struct sockaddr_in serv_addr, client_addr;

    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(serv_addr.sin_addr));
    serv_addr.sin_port = htons(std::stoi(argv[2]));

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    } else {
        std::cout << "Socket created" << std::endl;
    }

    message msg = {1, 0, 0};
    sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    sleep(3);
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        int department = rand() % NUM_DEPARTMENTS;
        std::cout << "Customer is in queue in department " << department << std::endl;
        message msg = {1, 1, department};
        sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

        // Receive message from server
        sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        int bytesReceived = recvfrom(sock, &msg, sizeof(msg), 0, (struct sockaddr*)&client_addr, &addr_len);
        if (bytesReceived < 0) {
            std::cerr << "Error receiving message from server: " << strerror(errno) << std::endl;
        } else {
            std::cout << "Customer is done shopping in department " << department << std::endl;
        }
        sleep(1);
    }

    std::cout << "Customer has left the store" << std::endl;

    return 0;
}
