#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <PORT>" << std::endl;
        return 1;
    }

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

    while (true) {
        char buffer[1024] = {0};
        int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived < 0) {
            std::cerr << "Error receiving message from server" << std::endl;
            break;
        } else if (bytesReceived == 0) {
            std::cout << "Server closed connection" << std::endl;
            break;
        } else {
            buffer[bytesReceived] = '\0'; // Добавляем нулевой символ в конец строки
            int request = std::stoi(buffer);
            std::cout << "Serving customer: " << request << std::endl;

            // Обработка запроса
            std::cout << "Processing ..." << std::endl;
            sleep(3); // Имитация обработки запроса
            // Отправка сообщения о завершении обработки на сервер
            std::string doneMessage = "Done processing request";
            send(sock, doneMessage.c_str(), doneMessage.size(), 0);
            std::cout << "Finished serving customer: " << request << std::endl;
        }
    }

    return 0;
}