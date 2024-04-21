#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <random>
#include <fstream>
#include <cstdlib>
#include <sys/wait.h>

const int NUM_DEPARTMENTS = 3;
int NUM_CUSTOMERS = 4;
sem_t *departments[NUM_DEPARTMENTS];
std::vector<pid_t> seller_pids;
std::ofstream out;
sem_t *customer_arrived[NUM_DEPARTMENTS];

void handle_sigint(int sig) {
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        sem_close(departments[i]);
        sem_unlink(("/sem" + std::to_string(i)).c_str());
        sem_close(customer_arrived[i]);
        sem_unlink(("/sem_arrived" + std::to_string(i)).c_str());
    }
    exit(0);
}


void seller(int dep_num) {
    while (true) {
        sem_wait(customer_arrived[dep_num]);  // Продавец ждет покупателя
        std::cout << "Seller in department " << dep_num + 1 << " is busy.\n";
        if (out.is_open()) {
            out << "Seller in department " << dep_num + 1 << " is busy.\n";
        }
        sleep(1);  // Симулируем время, необходимое для обслуживания покупателя
        std::cout << "Seller in department " << dep_num + 1 << " is free.\n";
        if (out.is_open()) {
            out << "Seller in department " << dep_num + 1 << " is free.\n";
        }
        sem_post(departments[dep_num]);
    }
}

void customer(int i) {
    std::cout << "Customer " << i << " has entered the store.\n";
    if (out.is_open()) {
        out << "Customer " << i << " has entered the store.\n";
    }
    srand(getpid());
    auto repeat = rand() % NUM_DEPARTMENTS + 1;
    for (int j = 0; j < repeat; ++j) {
        auto dep_num = rand() % NUM_DEPARTMENTS;
        sem_t *department = departments[dep_num];

        std::cout << "Customer " << i << " is in queue in department " << dep_num + 1 << ".\n";
        if (out.is_open()) {
            out << "Customer " << i << " is in queue in department " << dep_num + 1 << ".\n";
        }
        sem_post(customer_arrived[dep_num]);  // Покупатель сообщает о своем приходе
        sem_wait(department);
        sleep(1);
        std::cout << "Customer " << i << " is done shopping in department " << dep_num + 1 << ".\n";
        if (out.is_open()) {
            out << "Customer " << i << " is done shopping in department " << dep_num + 1 << ".\n";
        }
        sleep(1);
    }
    std::cout << "Customer " << i << " has left the store.\n";
    if (out.is_open()) {
        out << "Customer " << i << " has left the store.\n";
    }
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " NUM_CUSTOMERS [OUTPUT_FILE]\n";
        return 1;
    }

    NUM_CUSTOMERS = std::atoi(argv[1]);
    if (argc == 3) {
        out.open(argv[2]);
        if (!out) {
            std::cerr << "Failed to open output file " << argv[2] << "\n";
            return 1;
        } else {
            std::cout << "Output is also redirected to " << argv[2] << "\n";
        }
    }

    signal(SIGINT, handle_sigint);

    // Создаем семафоры и процессы для каждого отдела
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        departments[i] = sem_open(("/sem" + std::to_string(i)).c_str(), O_CREAT, 0644, 1);
        customer_arrived[i] = sem_open(("/sem_arrived" + std::to_string(i)).c_str(), O_CREAT, 0644,
                                       0);  // Инициализируем второй семафор
        if (departments[i] == SEM_FAILED || customer_arrived[i] == SEM_FAILED) {
            std::cerr << "Failed to create semaphore\n";
            return 1;
        }
        pid_t pid = fork();
        if (pid == 0) {
            seller(i);
            return 0;
        } else {
            seller_pids.push_back(pid);  // Добавляем PID продавца в список
        }
    }

    // Создаем процессы для каждого покупателя
    for (int i = 1; i <= NUM_CUSTOMERS; ++i) {
        if (fork() == 0) {
            customer(i);
            return 0;
        }
    }

    // Ждем завершения всех процессов
    for (int i = 0; i < NUM_CUSTOMERS; ++i) {
        wait(NULL);
    }
    std::cout << "All customers have left the store.\n";
    if (out.is_open()) {
        out << "All customers have left the store.\n";
    }
    for (pid_t pid: seller_pids) {
        kill(pid, SIGINT);
    }
    // Удаляем семафоры
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        sem_close(departments[i]);
        sem_unlink(("/sem" + std::to_string(i)).c_str());
        sem_close(customer_arrived[i]);
        sem_unlink(("/sem_arrived" + std::to_string(i)).c_str());
    }

    return 0;
}