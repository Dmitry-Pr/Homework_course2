#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <fstream>

const int NUM_DEPARTMENTS = 3;

std::ofstream out;

void seller(int dep_num, int all_customers_done, int customer_arrived, int department) {

    out.open("output.txt", std::ios::app); // Открытие файла для записи

    while (true) {
        sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;  // Уменьшаем значение семафора на 1
        sem_op.sem_flg = 0;
        // Проверяем, ушли ли все покупатели
        int sem_val = semctl(all_customers_done, 0, GETVAL, NULL);
        if (sem_val == 1) {
            break;  // Если все покупатели ушли, продавец прекращает работу
        }
        semop(customer_arrived, &sem_op, 1);  // Продавец ждет покупателя
        std::cout << "Seller in department " << dep_num + 1 << " is busy" << std::endl;
        if (out.is_open()) {
            out << "Seller in department " << dep_num + 1 << " is busy" << std::endl;
        }
        sleep(1);  // Симулируем время, необходимое для обслуживания покупателя
        std::cout << "Seller in department " << dep_num + 1 << " is free" << std::endl;
        if (out.is_open()) {
            out << "Seller in department " << dep_num + 1 << " is free" << std::endl;
        }
        sem_op.sem_op = 1;
        semop(department, &sem_op, 1);  // Продавец сообщает о завершении обслуживания
    }

    out.close(); // Закрытие файла

    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " dep_num all_customers_done" << std::endl;
        return 1;
    }

    int dep_num = std::atoi(argv[1]);
    int all_customers_done = std::atoi(argv[2]);
    int customer_arrived = std::atoi(argv[3]);
    int department = std::atoi(argv[4]);
    seller(dep_num, all_customers_done, customer_arrived, department);

    return 0;
}