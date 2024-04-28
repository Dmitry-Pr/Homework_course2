#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <cstring>
#include <fstream>
#include <sys/wait.h>
#include <fcntl.h>

const int NUM_DEPARTMENTS = 3;
int NUM_CUSTOMERS = 4;
sem_t *customer_arrived[NUM_DEPARTMENTS];
sem_t *departments[NUM_DEPARTMENTS];
sem_t *all_customers_done;
int pipefd[2];  // Добавляем идентификатор канала

struct my_msgbuf {
    long mtype;
    int mtext[2 * NUM_DEPARTMENTS];
};

void cleanup(int sig) {
    // Удаление семафоров
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        sem_close(customer_arrived[i]);
        sem_unlink(("/customer_arrived" + std::to_string(i)).c_str());
        sem_close(departments[i]);
        sem_unlink(("/departments" + std::to_string(i)).c_str());
    }
    sem_close(all_customers_done);
    sem_unlink("/all_customers_done");

    // Закрываем канал
    close(pipefd[0]);
    close(pipefd[1]);

    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " NUM_CUSTOMERS" << std::endl;
        return 1;
    }
    signal(SIGINT, cleanup);
    NUM_CUSTOMERS = std::atoi(argv[1]);
    std::ofstream out;
    out.open("output.txt");
    out.close();

    // Создание и инициализация семафора all_customers_done
    all_customers_done = sem_open("/all_customers_done", O_CREAT, 0666, 0);

    // Создаем канал
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // Создаем процессы для каждого отдела
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        customer_arrived[i] = sem_open(("/customer_arrived" + std::to_string(i)).c_str(), O_CREAT, 0666, 0);
        departments[i] = sem_open(("/departments" + std::to_string(i)).c_str(), O_CREAT, 0666, 0);
        pid_t pid = fork();
        if (pid == 0) {
            execl("./seller", "./seller", std::to_string(i).c_str(), "/all_customers_done",
                  ("/customer_arrived" + std::to_string(i)).c_str(), ("/departments" + std::to_string(i)).c_str(),
                  NULL);
            exit(0);
        }
    }

    // Создаем процессы для каждого покупателя
    for (int i = 1; i <= NUM_CUSTOMERS; ++i) {
        if (fork() == 0) {
            struct my_msgbuf msg;
            msg.mtype = 1;  // Установка типа сообщения
            memcpy(msg.mtext, customer_arrived,
                   sizeof(int) * NUM_DEPARTMENTS);  // Копирование массива customer_arrived в сообщение
            memcpy(msg.mtext + sizeof(int) * NUM_DEPARTMENTS, departments,
                   sizeof(int) * NUM_DEPARTMENTS);  // Копирование массива departments в сообщение

            // Отправка сообщения через канал
            write(pipefd[1], &msg, sizeof(msg));

            execl("./customer", "./customer", std::to_string(i).c_str(), std::to_string(pipefd[0]).c_str(), NULL);
            exit(0);
        }
    }

    // Ждем завершения всех процессов
    for (int i = 0; i < NUM_CUSTOMERS; ++i) {
        wait(NULL);
    }
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        sem_post(all_customers_done);  // Уведомляем продавцов о завершении работы
    }

    // Все покупатели ушли, уведомляем продавцов
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        sem_post(customer_arrived[i]);
    }
    int sem_val = 3;
    while (sem_val < 6) {
        sem_getvalue(all_customers_done, &sem_val);
    }
    cleanup(0);

    return 0;
}