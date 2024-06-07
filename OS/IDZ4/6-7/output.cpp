#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include "Message.cpp"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <SHOP_IP> <SHOP_PORT>" << std::endl;
        return 1;
    }

    int sock = 0;
    struct sockaddr_in shop_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    shop_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(shop_addr.sin_addr));
    shop_addr.sin_port = htons(std::stoi(argv[2]));

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    } else {
        std::cout << "Socket created" << std::endl;
    }
    message msg = {2, 0, 0, ""};
    sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr*)&shop_addr, sizeof(shop_addr));

    while (true) {
        int bytesReceived = recvfrom(sock, &msg, sizeof(msg), 0, (struct sockaddr*)&client_addr, &client_len);
        if (bytesReceived < 0) {
            std::cerr << "Error receiving message from client" << std::endl;
            break;
        } else {
            msg.text[bytesReceived] = '\0'; // Null terminate the string
            std::cout << msg.text << std::endl;
        }
    }

    return 0;
}