#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <arpa/inet.h>

const int NUM_DEPARTMENTS = 3;
std::vector<int> departments(NUM_DEPARTMENTS, 0);

void handle_customer(int department) {
    while (true) {
        if (departments[department] == 0) {
            departments[department] = 1;
            std::cout << "Serving customer in department " << department + 1 << std::endl;
            sleep(1); // Simulate serving time
            std::cout << "Done serving customer in department " << department + 1 << std::endl;
            departments[department] = 0;
            break;
        }
        sleep(1); // Wait for the department to be free
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <PORT>" << std::endl;
        return 1;
    }
    int serversock;
    if ((serversock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return 1;
    } else {
        std::cout << "Socket created" << std::endl;
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(address.sin_addr));
    address.sin_port = htons(std::stoi(argv[2]));

    bind(serversock, (struct sockaddr *) &address, sizeof(address));
    if (listen(serversock, SOMAXCONN) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return 1;
    } else {
        std::cout << "Listening on " << argv[1] << ":" << argv[2] << std::endl;
    }

    while (true) {
        std::cout << "Waiting for customers..." << std::endl;
        int new_socket = accept(serversock, nullptr, nullptr);
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        int department = atoi(buffer);
        std::thread(handle_customer, department).detach();
    }

    return 0;
}