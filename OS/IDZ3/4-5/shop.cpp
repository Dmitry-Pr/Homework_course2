#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <csignal>
#include <arpa/inet.h>

const int NUM_DEPARTMENTS = 3;
std::vector<int> departments(NUM_DEPARTMENTS, 0);
int server_fd = 0;

void handle_client(int client_sock, int customer_num) {
    char buffer[1024] = {0};
    while (true) {
        int bytesReceived = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived < 0) {
            std::cerr << "Error receiving message from client" << std::endl;
            break;
        } else if (bytesReceived == 0) {
            std::cout << "Client closed connection" << std::endl;
            break;
        } else {
            buffer[bytesReceived] = '\0'; // Добавляем нулевой символ в конец строки
            std::cout << "Received message from client: " << buffer << std::endl;

            // Получаем номер продавца от клиента
            int seller_num = std::stoi(buffer);

            // Проверяем, что номер продавца валидный
            if (seller_num < 0 || seller_num >= NUM_DEPARTMENTS) {
                std::cerr << "Invalid seller number received from client" << std::endl;
                continue;
            }

            // Отправляем сообщение продавцу
            std::string message = std::to_string(customer_num);
            send(departments[seller_num], message.c_str(), message.size(), 0);

            // Получаем ответ от продавца
            char seller_buffer[1024] = {0};
            int seller_bytesReceived = recv(departments[seller_num], seller_buffer, sizeof(seller_buffer) - 1, 0);
            if (seller_bytesReceived < 0) {
                std::cerr << "Error receiving message from seller" << std::endl;
            } else {
                seller_buffer[seller_bytesReceived] = '\0'; // Добавляем нулевой символ в конец строки
                std::cout << "Received message from seller: " << seller_buffer << std::endl;

                // Отправляем ответ продавца клиенту
                send(client_sock, seller_buffer, seller_bytesReceived, 0);
            }
        }
    }
}

void finish(int signum) {
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        close(departments[i]);
    }
    close(server_fd);
    exit(0);

}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <PORT>" << std::endl;
        return 1;
    }

    signal(SIGINT, finish);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(address.sin_addr));
    address.sin_port = htons(std::stoi(argv[2]));

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    } else {
        std::cout << "Socket created" << std::endl;
    }

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        std::cout << "Bind failed" << std::endl;
        return -1;
    } else {
        std::cout << "Bind successful" << std::endl;
    }
    if (listen(server_fd, SOMAXCONN) < 0) {
        std::cout << "Listen failed" << std::endl;
        return -1;
    } else {
        std::cout << "Server listening" << std::endl;
    }

    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        int department = i;
        int new_socket = accept(server_fd, (struct sockaddr *) nullptr, nullptr);
        if (new_socket < 0) {
            std::cout << "Accept failed" << std::endl;
            return -1;
        } else {
            std::cout << "Accepted connection" << std::endl;
        }
        departments[department] = new_socket;
    }
    int customer_num = -1;
    while (true) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int new_socket = accept(server_fd, (struct sockaddr *) &client_address, &client_address_len);
        customer_num++;
        if (new_socket < 0) {
            std::cout << "Accept failed" << std::endl;
            break;
        } else {
            std::cout << "Accepted new connection" << std::endl;
            pid_t pid = fork();
            if (pid < 0) {
                std::cerr << "Fork failed" << std::endl;
                break;
            } else if (pid == 0) {
                // Это код дочернего процесса

                handle_client(new_socket, customer_num);

                exit(0);
            }
        }
    }
}