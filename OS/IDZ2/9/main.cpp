#include <iostream>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <cstring>
#include <fstream>
#include <sys/wait.h>

const int NUM_DEPARTMENTS = 3;
int NUM_CUSTOMERS = 4;
int customer_arrived[NUM_DEPARTMENTS];
int departments[NUM_DEPARTMENTS];
int all_customers_done;
int msqid;

struct my_msgbuf {
    long mtype;
    int mtext[2 * NUM_DEPARTMENTS];
};

void cleanup(int sig) {
    // Удаление семафоров
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        semctl(customer_arrived[i], 0, IPC_RMID);
        semctl(departments[i], 0, IPC_RMID);
    }
    semctl(all_customers_done, 0, IPC_RMID);

    // Удаление очереди сообщений
    msgctl(msqid, IPC_RMID, NULL);
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
    all_customers_done = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (all_customers_done == -1) {
        perror("semget failed");
        exit(1);
    }
    if (semctl(all_customers_done, 0, SETVAL, 0) == -1) {
        perror("semctl failed");
        exit(1);
    }

    // Создание очереди сообщений
    msqid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Создаем процессы для каждого отдела
    for (int i = 0; i < NUM_DEPARTMENTS; ++i) {
        customer_arrived[i] = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
        if (customer_arrived[i] == -1) {
            perror("semget failed");
            exit(1);
        }
        if (semctl(customer_arrived[i], 0, SETVAL, 0) == -1) {
            perror("semctl failed");
            exit(1);
        }
        departments[i] = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
        if (departments[i] == -1) {
            perror("semget failed");
            exit(1);
        }
        if (semctl(departments[i], 0, SETVAL, 0) == -1) {
            perror("semctl failed");
            exit(1);
        }
        pid_t pid = fork();
        if (pid == 0) {
            execl("./seller", "./seller", std::to_string(i).c_str(), std::to_string(all_customers_done).c_str(), std::to_string(customer_arrived[i]).c_str(), std::to_string(departments[i]).c_str(), std::to_string(msqid).c_str(), NULL);
            exit(0);
        }
    }

    // Создаем процессы для каждого покупателя
    for (int i = 1; i <= NUM_CUSTOMERS; ++i) {
        if (fork() == 0) {
            struct my_msgbuf msg;
            msg.mtype = 1;  // Установка типа сообщения
            memcpy(msg.mtext, customer_arrived, sizeof(int) * NUM_DEPARTMENTS);  // Копирование массива customer_arrived в сообщение
            memcpy(msg.mtext + sizeof(int) * NUM_DEPARTMENTS, departments, sizeof(int) * NUM_DEPARTMENTS);  // Копирование массива departments в сообщение

            // Отправка сообщения
            if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) == -1) {
                perror("msgsnd failed");
                exit(1);
            }
            execl("./customer", "./customer", std::to_string(i).c_str(), std::to_string(msqid).c_str(), NULL, NULL);
            exit(0);
        }
    }

    // Ждем завершения всех процессов
    for (int i = 0; i < NUM_CUSTOMERS; ++i) {
        wait(NULL);
    }

    // Все покупатели ушли, уведомляем продавцов
    sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = 3;  // Увеличиваем значение семафора на 3
    sem_op.sem_flg = 0;
    semop(all_customers_done, &sem_op, 1);

    for(int i = 0; i < NUM_DEPARTMENTS; ++i) {
        sem_op.sem_op = 1;
        semop(customer_arrived[i], &sem_op, 1);
    }

    sem_op.sem_op = -6;
    semop(all_customers_done, &sem_op, 1);
    cleanup(0);

    return 0;
}