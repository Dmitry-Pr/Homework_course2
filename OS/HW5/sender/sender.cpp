#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

volatile int bit = 0;
volatile int bit_received = 0;

void sigusr1_handler(int signo) {
    bit_received = 1;
}


int main() {
    pid_t receiver_pid;

    std::cout << "PID отправителя: " << getpid() << std::endl;
    std::cout << "Введите PID получателя: ";
    std::cin >> receiver_pid;

    (void)signal(SIGUSR1, sigusr1_handler);


    int data;
    printf("Введите целое число: ");
    std::cin >> data;
    for (int i = 7; i >= 0; i--) {
        if ((data >> i) & 1) {
            kill(receiver_pid, SIGUSR1);
            std::cout << 1;
        } else {
            kill(receiver_pid, SIGUSR2);
            std::cout << 0;
        }
        while (!bit_received);
        bit_received = 0;
    }
    kill(receiver_pid, SIGINT);
    std::cout << std::endl;
    std::cout << "Отправка завершена";

    return 0;
}
