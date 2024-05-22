#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <arpa/inet.h>

const int NUM_DEPARTMENTS = 3;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <PORT>" << std::endl;
        return 1;
    }

    srand(time(0));
    int sock = 0;
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(serv_addr.sin_addr));
    serv_addr.sin_port = htons(std::stoi(argv[2]));

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    } else {
        std::cout << "Socket created" << std::endl;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection failed" << std::endl;
        return -1;
    } else {
        std::cout << "Connected to server" << std::endl;
    }
    char t[1024] = {0};
    sprintf(t, "%d", 1);
    send(sock, t, strlen(t), 0);
    sleep(4);
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        int department = rand() % NUM_DEPARTMENTS;
        std::cout << "Customer is in queue in department " << department << std::endl;
        char buffer[1024] = {0};
        sprintf(buffer, "%d", department);
        send(sock, buffer, strlen(buffer), 0);

        // Получение сообщения от сервера
        char recvBuffer[1024] = {0};
        int bytesReceived = recv(sock, recvBuffer, sizeof(recvBuffer) - 1, 0);
        if (bytesReceived < 0) {
            std::cerr << "Error receiving message from server" << std::endl;
        } else {
            std::cout << "Customer is done shopping in department " << department << std::endl;
        }
        sleep(2);
    }

    std::cout << "Customer has left the store" << std::endl;
    close(sock);
    return 0;
}