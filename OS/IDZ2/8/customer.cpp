#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <fstream>
#include <sys/shm.h>
#include <cstring>

const int NUM_DEPARTMENTS = 3;
int customer_arrived[NUM_DEPARTMENTS];
int departments[NUM_DEPARTMENTS];
std::ofstream out;

void customer(int i) {

    out.open("output.txt", std::ios::app); // Открытие файла для записи

    for (int j = 0; j < NUM_DEPARTMENTS; ++j) {
        auto dep_num = rand() % NUM_DEPARTMENTS;

        std::cout << "Customer " << i << " is in queue in department " << dep_num + 1 << std::endl;
        if (out.is_open()) {
            out << "Customer " << i << " is in queue in department " << dep_num + 1 << std::endl;
        }
        sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(customer_arrived[dep_num], &sem_op, 1);  // Покупатель сообщает о своем приходе
        sem_op.sem_op = -1;
        semop(departments[dep_num], &sem_op, 1);
        sleep(1);
        std::cout << "Customer " << i << " is done shopping in department " << dep_num + 1 << std::endl;
        if (out.is_open()) {
            out << "Customer " << i << " is done shopping in department " << dep_num + 1 << std::endl;
        }
        sleep(1);
    }
    std::cout << "Customer " << i << " has left the store" << std::endl;
    if (out.is_open()) {
        out << "Customer " << i << " has left the store" << std::endl;
    }
    exit(0);

    out.close(); // Закрытие файла

    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " customer_id" << std::endl;
        return 1;
    }

    int customer_id = std::atoi(argv[1]);
    int fd = std::atoi(argv[2]);
    int *shared_memory = (int *) shmat(fd, NULL, 0);
    if (shared_memory == (int *) -1) {
        perror("shmat failed");
        exit(1);
    }

// Чтение данных массивов из общей памяти
    memcpy(customer_arrived, shared_memory, sizeof(int) * NUM_DEPARTMENTS);
    memcpy(departments, shared_memory + sizeof(int) * NUM_DEPARTMENTS, sizeof(int) * NUM_DEPARTMENTS);
    customer(customer_id);

    return 0;
}