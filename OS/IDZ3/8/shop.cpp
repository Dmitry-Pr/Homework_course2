#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <csignal>
#include <arpa/inet.h>
#include <sstream>
#include <sys/sem.h>
#include <map>



const int NUM_DEPARTMENTS = 3;
std::vector<int> departments(NUM_DEPARTMENTS, 0);
pthread_t threads[NUM_DEPARTMENTS];
std::vector <pthread_t> client_threads;
std::vector<int> output_sockets;
int server_fd = 0;
int output_socket = 0;
std::map<int, pthread_mutex_t> socket_mutexes;

void *handle_client(void *arg) {
    int client_sock = ((int *) arg)[0];
    int customer_num = ((int *) arg)[1];
    delete[] (int *) arg;
    pthread_mutex_t socket_mutex;
    pthread_mutex_init(&socket_mutex, NULL);
    socket_mutexes[client_sock] = socket_mutex;

    char buffer[1024] = {0};
    while (true) {
        // Перед вызовом recv()
        pthread_mutex_lock(&socket_mutexes[client_sock]);

        int bytesReceived = recv(client_sock, buffer, sizeof(buffer) - 1, 0);

        // После вызова recv()
        pthread_mutex_unlock(&socket_mutexes[client_sock]);

        if (bytesReceived < 0) {
            std::cerr << "Error receiving message from client" << std::endl;
            break;
        } else if (bytesReceived == 0) {
            std::cout << "Client closed connection" << std::endl;
            std::string output = "Customer " + std::to_string(customer_num) + " has left the store";
            // Отправка сообщения всем клиентам вывода информации
            for (int output_socket : output_sockets) {
                send(output_socket, output.c_str(), output.size(), 0);
            }
            break;
        } else {
            buffer[bytesReceived] = '\0'; // Добавляем нулевой символ в конец строки
            std::cout << "Received message from client: " << buffer << std::endl;
            std::string output = "Customer " + std::to_string(customer_num) + " is in queue in department " + buffer;
            // Отправка сообщения всем клиентам вывода информации
            for (int output_socket : output_sockets) {
                send(output_socket, output.c_str(), output.size(), 0);
            }
            // Получаем номер продавца от клиента
            int seller_num = std::stoi(buffer);

            // Проверяем, что номер продавца валидный
            if (seller_num < 0 || seller_num >= NUM_DEPARTMENTS) {
                std::cerr << "Invalid seller number received from client" << std::endl;
                continue;
            }

            // Отправляем сообщение продавцу
            char message[1024] = {0};
            sprintf(message, "%d\n%d\n", customer_num, client_sock);
            send(departments[seller_num], message, strlen(message), 0);

            // Перед вызовом send()
            pthread_mutex_lock(&socket_mutexes[client_sock]);

//            // Получаем ответ от продавца
//            char seller_buffer[1024] = {0};
//            int seller_bytesReceived = recv(departments[seller_num], seller_buffer, sizeof(seller_buffer) - 1, 0);
//            if (seller_bytesReceived < 0) {
//                std::cerr << "Error receiving message from seller" << std::endl;
//            } else {
//                seller_buffer[seller_bytesReceived] = '\0'; // Добавляем нулевой символ в конец строки
//                std::cout << "Received message from seller: " << seller_buffer << std::endl;
//                std::string output =
//                        "Customer " + std::to_string(customer_num) + " is done shopping in department " + buffer;
//                send(output_socket, output.c_str(), output.size(), 0);
//                // Отправляем ответ продавца клиенту
//                send(client_sock, seller_buffer, seller_bytesReceived, 0);
//            }

        }
    }
    return NULL;
}

void *answer_to_client(void *arg) {
    int seller_num = *((int *) arg);
    free(arg);
    while (true) {
        // Получаем ответ от продавца
        char seller_buffer[1024] = {0};
        int seller_bytesReceived = recv(departments[seller_num], seller_buffer, sizeof(seller_buffer) - 1, 0);
        if (seller_bytesReceived < 0) {
            std::cerr << "Error receiving message from seller" << std::endl;
        } else {
            seller_buffer[seller_bytesReceived] = '\0'; // Добавляем нулевой символ в конец строки
            std::cout << "Received message from seller: " << seller_buffer << std::endl;
            std::string str(seller_buffer);
            std::istringstream iss(str);
            std::string line;
            std::getline(iss, line);
            int customer_num = std::stoi(line);
            std::getline(iss, line);
            int client_sock = std::stoi(line);
            std::string output = "Customer " + std::to_string(customer_num) + " is done shopping in department " +
                                 std::to_string(seller_num);
            // Отправка сообщения всем клиентам вывода информации
            for (int output_socket : output_sockets) {
                send(output_socket, output.c_str(), output.size(), 0);
            }
            // Отправляем ответ продавца клиенту
            int bytesSent = send(client_sock, seller_buffer, seller_bytesReceived, 0);
            if (bytesSent < 0) {
                std::string merror = "Error sending message to client: " + std::to_string(client_sock);
                perror(merror.c_str());
            } else {
                std::cout << "Sent message to client: " << seller_buffer << std::endl;
            }

            // После вызова send()
            pthread_mutex_unlock(&socket_mutexes[client_sock]);
        }
    }
    return NULL;
}

void finish(int signum) {
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        close(departments[i]);
    }
    close(server_fd);
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        pthread_cancel(threads[i]);
    }
    for (int i = 0; i < client_threads.size(); ++i) {
        pthread_cancel(client_threads[i]);
    }
    for (auto &pair: socket_mutexes) {
        pthread_mutex_destroy(&pair.second);
    }
    for (int output_socket : output_sockets) {
        close(output_socket);
    }
    exit(0);

}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <IP> <PORT> <NUM_CUSTOMERS>" << std::endl;
        return 1;
    }

    signal(SIGINT, finish);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(address.sin_addr));
    address.sin_port = htons(std::stoi(argv[2]));
    int num_customers = std::stoi(argv[3]);

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

    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        int *arg = new int;
        if (arg == NULL) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }

        *arg = i;
        int ret = pthread_create(&threads[i], NULL, answer_to_client, arg);

        if (ret != 0) {
            std::cerr << "Error: pthread_create() failed\n";
            exit(EXIT_FAILURE);
        }
    }
    // Обработка подключения новых клиентов
    while (customer_num < num_customers - 1) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int new_socket = accept(server_fd, (struct sockaddr *) &client_address, &client_address_len);
        customer_num++;
        std::cout << new_socket << std::endl;
        if (new_socket < 0) {
            std::cout << "Accept failed" << std::endl;
            break;
        } else {
            std::cout << "Accepted new connection" << std::endl;
            pthread_t thread;
            int *arg = new int[2];
            if (arg == NULL) {
                fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
                exit(EXIT_FAILURE);
            }

            arg[0] = new_socket;
            arg[1] = customer_num;
            int ret = pthread_create(&thread, NULL, handle_client, arg);

            if (ret != 0) {
                std::cerr << "Error: pthread_create() failed\n";
                break;
            }
        }
    }
    // Подключение клиентов для вывода информации
    while (true) {
        int new_output_socket = accept(server_fd, (struct sockaddr *) nullptr, nullptr);
        if (new_output_socket < 0) {
            std::cout << "Accept failed" << std::endl;
            break;
        } else {
            std::cout << "Connected output client" << std::endl;
            output_sockets.push_back(new_output_socket);
        }
    }
}