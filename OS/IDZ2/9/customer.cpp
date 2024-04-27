#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <fstream>
#include <cstring>
#include <sys/msg.h>

const int NUM_DEPARTMENTS = 3;
int customer_arrived[NUM_DEPARTMENTS];
int departments[NUM_DEPARTMENTS];
std::ofstream out;

struct my_msgbuf {
    long mtype;
    int mtext[2 * NUM_DEPARTMENTS];
};

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
    int msqid = std::atoi(argv[2]);
    struct my_msgbuf msg;
    if (msgrcv(msqid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }

// Чтение данных массивов из общей памяти
    memcpy(customer_arrived, msg.mtext, sizeof(int) * NUM_DEPARTMENTS);
    memcpy(departments, msg.mtext + sizeof(int) * NUM_DEPARTMENTS, sizeof(int) * NUM_DEPARTMENTS);
    customer(customer_id);

    return 0;
}