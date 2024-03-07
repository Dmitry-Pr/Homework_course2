#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>

volatile int bit = 0;

void sigusr1_handler(int signo) {
    bit = (bit << 1) | 1;
    std::cout << 1;
    kill(getppid(), SIGUSR1);

}

void sigusr2_handler(int signo) {
    bit = bit << 1;
    std::cout << 0;
    kill(getppid(), SIGUSR1);  // Acknowledge the sender

}

void sigint_handler(int signo) {
    std::cout << bit;
    std::cout << "Получение завершено";
    int a;
    std::cin >> a;
    exit(0);
}

int main() {
    pid_t sender_pid;

    std::cout << "PID получателя: " << getpid() << std::endl;
    std::cout << "Введите PID отправителя: ";
    std::cin >> sender_pid;
    std::cout << "Готов к получению\n";
    (void)signal(SIGUSR1, sigusr1_handler);
    (void)signal(SIGUSR2, sigusr2_handler);
    (void)signal(SIGINT, sigint_handler);

    while (true);


    return 0;
}
