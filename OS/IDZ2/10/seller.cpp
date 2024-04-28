#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <mqueue.h>
#include <cstring>
#include <fstream>
#include <fcntl.h>

const int NUM_DEPARTMENTS = 3;
sem_t *customer_arrived[NUM_DEPARTMENTS];
sem_t *departments[NUM_DEPARTMENTS];
sem_t *all_customers_done;
mqd_t mq;

struct my_msgbuf {
    long mtype;
    int mtext[2 * NUM_DEPARTMENTS];
};

void seller(int dep_num, sem_t *all_customers_done, sem_t *customer_arrived, sem_t *department) {
    std::ofstream out;
    out.open("output.txt", std::ios::app); // Открытие файла для записи

    while (true) {
        sem_wait(customer_arrived);
        int sem_val;
        sem_getvalue(all_customers_done, &sem_val);
        if (sem_val > 0) {
            sem_post(all_customers_done);
            break;
        }
        std::cout << "Seller in department " << dep_num + 1 << " is busy" << std::endl;
        if (out.is_open()) {
            out << "Seller in department " << dep_num + 1 << " is busy" << std::endl;
        }
        sleep(1);
        std::cout << "Seller in department " << dep_num + 1 << " is free" << std::endl;
        if (out.is_open()) {
            out << "Seller in department " << dep_num + 1 << " is free" << std::endl;
        }
        sem_post(department);
        sleep(1);
    }
    out.close(); // Закрытие файла
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " DEP_NUM ALL_CUSTOMERS_DONE CUSTOMER_ARRIVED DEPARTMENT" << std::endl;
        return 1;
    }
    int dep_num = std::atoi(argv[1]);
    char* all_customers_done_name = argv[2];
    char* customer_arrived_name = argv[3];
    char* department_name = argv[4];

    // Открытие семафоров
    all_customers_done = sem_open(all_customers_done_name, O_RDWR);
    customer_arrived[dep_num] = sem_open(customer_arrived_name, O_RDWR);
    departments[dep_num] = sem_open(department_name, O_RDWR);

    seller(dep_num, all_customers_done, customer_arrived[dep_num], departments[dep_num]);

    return 0;
}