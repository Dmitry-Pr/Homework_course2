#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <csignal>
#include <arpa/inet.h>
#include <queue>
#include <unordered_map>
#include "Message.cpp"

const int NUM_DEPARTMENTS = 3;
std::vector<struct sockaddr_in> departments;
std::vector<struct sockaddr_in> customers;
std::vector<struct sockaddr_in> outputs;
int server_sock = 0;
int customer_id = 0;

void handle_command(message &mess, sockaddr_in from_addr) {
    if (mess.senderType == 0) { 
        // from seller
        if (mess.messageType == 0) { 
            // Connection message
            std::cout << "Seller connected to shop" << std::endl;
            for (int i = 0; i < outputs.size(); i++) {
                message new_message = {0, 0, 0, "Seller connected to shop"};
                sendto(server_sock, &new_message, sizeof(new_message), 0, (struct sockaddr *) &outputs[i],
                       sizeof(outputs[i]));
            }
            departments.push_back(from_addr);
        } else if (mess.messageType == 1) { 
            // Served customer
            int customer = mess.whomToSend;
            std::string text = "Customer " + std::to_string(customer) + " has been served";
            std::cout << text << std::endl;
            for (int i = 0; i < outputs.size(); i++) {
                message new_message = {0, 0, 0, ""};
                std::strcpy(new_message.text, text.c_str());
                sendto(server_sock, &new_message, sizeof(new_message), 0, (struct sockaddr *) &outputs[i],
                       sizeof(outputs[i]));
            }
            sendto(server_sock, &mess, sizeof(mess), 0, (struct sockaddr *) &customers[customer],
                   sizeof(customers[customer]));
        }
    } else if (mess.senderType == 1) {
        // from customer
        if (mess.messageType == 0) {
            // Connection message
            std::string text = "Customer " + std::to_string(customer_id) + " connected to shop";
            std::cout << text << std::endl;
            for (int i = 0; i < outputs.size(); i++) {
                message new_message = {0, 0, 0, ""};
                std::strcpy(new_message.text, text.c_str());
                sendto(server_sock, &new_message, sizeof(new_message), 0, (struct sockaddr *) &outputs[i],
                       sizeof(outputs[i]));
            }
            customers.push_back(from_addr);
        } else if (mess.messageType == 1) {
            // to seller
            int department = mess.whomToSend;
            int customer = -1;

            for (int i = 0; i < customers.size(); i++) {
                if (memcmp(&customers[i], &from_addr, sizeof(from_addr)) == 0) {
                    customer = i;
                    break;
                }
            }
            std::string text = "Customer " + std::to_string(customer) + " is in queue in department " + std::to_string(department);
            std::cout << text << std::endl;
            message new_message = {1, 1, customer, ""};
            std::strcpy(new_message.text, text.c_str());
            for (int i = 0; i < outputs.size(); i++) {
                sendto(server_sock, &new_message, sizeof(new_message), 0, (struct sockaddr *) &outputs[i],
                       sizeof(outputs[i]));
            }
            sendto(server_sock, &new_message, sizeof(new_message), 0, (struct sockaddr *) &departments[department],
                   sizeof(departments[department]));
        }
    } else if (mess.senderType == 2) {
        // from output
        if (mess.messageType == 0) {
            // Connection message
            std::cout << "Output connected to shop" << std::endl;
            outputs.push_back(from_addr);
        }
    }
}


void finish(int signum) {
    close(server_sock);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <PORT>" << std::endl;
        return 1;
    }

    signal(SIGINT, finish);
    struct sockaddr_in address, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    address.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(address.sin_addr));
    address.sin_port = htons(std::stoi(argv[2]));

    if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    } else {
        std::cout << "Socket created" << std::endl;
    }

    if (bind(server_sock, (struct sockaddr *) &address, sizeof(address)) < 0) {
        std::cout << "Bind failed" << std::endl;
        return -1;
    } else {
        std::cout << "Bind successful" << std::endl;
    }

    while (true) {
        message mess;
        int bytesReceived = recvfrom(server_sock, &mess, sizeof(mess), 0, (struct sockaddr *) &client_addr,
                                     &client_addr_len);
        if (bytesReceived < 0) {
            std::cerr << "Error receiving message from client" << std::endl;
            break;
        } else {
            handle_command(mess, client_addr);
        }
    }
}
