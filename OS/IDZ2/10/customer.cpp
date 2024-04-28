#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <cstring>
#include <fstream>
#include <fcntl.h>

const int NUM_DEPARTMENTS = 3;
sem_t *customer_arrived[NUM_DEPARTMENTS];
sem_t *departments[NUM_DEPARTMENTS];
int pipefd[2];

struct my_msgbuf {
    long mtype;
    int mtext[2 * NUM_DEPARTMENTS];
};

void customer(int i) {
    std::ofstream out;
    out.open("output.txt", std::ios::app); // Открытие файла для записи

    std::cout << "Customer " << i << " has entered the store" << std::endl;
    if (out.is_open()) {
        out << "Customer " << i << " has entered the store" << std::endl;
    }
    srand(getpid());
    auto repeat = rand() % NUM_DEPARTMENTS + 1;
    for (int j = 0; j < repeat; ++j) {
        auto dep_num = rand() % NUM_DEPARTMENTS;
        sem_t *department = departments[dep_num];

        std::cout << "Customer " << i << " is in queue in department " << dep_num + 1 << std::endl;
        if (out.is_open()) {
            out << "Customer " << i << " is in queue in department " << dep_num + 1 << std::endl;
        }
        sem_post(customer_arrived[dep_num]);  // Покупатель сообщает о своем приходе
        sem_wait(department);
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
    out.close(); // Закрытие файла
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " CUSTOMER_ID PIPEFD" << std::endl;
        return 1;
    }
    int customer_id = std::atoi(argv[1]);
    pipefd[0] = std::atoi(argv[2]);

    struct my_msgbuf msg;
    if (read(pipefd[0], &msg, sizeof(struct my_msgbuf)) == -1) {
        perror("read failed");
        exit(1);
    }

    memcpy(customer_arrived, msg.mtext, sizeof(int) * NUM_DEPARTMENTS);
    memcpy(departments, msg.mtext + sizeof(int) * NUM_DEPARTMENTS, sizeof(int) * NUM_DEPARTMENTS);

    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        customer_arrived[i] = sem_open(("/customer_arrived" + std::to_string(i)).c_str(), O_RDWR);
        departments[i] = sem_open(("/departments" + std::to_string(i)).c_str(), O_RDWR);
    }

    customer(customer_id);

    return 0;
}