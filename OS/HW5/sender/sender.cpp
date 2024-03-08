#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <bitset>

bool ready = 0;

void handler(int signum) {
    if (signum == SIGUSR1) {
        ready = true;
    } else if (signum == SIGUSR2) {
        std::cout << "Ошибка при передаче бита" << std::endl;
        exit(1);
    }
}

int main() {
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    std::cout << getpid() << std::endl;
    pid_t pid_receiver;
    std::cout << "Введите PID приемника: ";
    std::cin >> pid_receiver;

    int number;
    std::cout << "Введите число: ";
    std::cin >> number;

    std::string binary_number = std::bitset<32>(number).to_string();
    for (char bit: binary_number) {
        if (bit == '0') {
            std::cout << 0;
            kill(pid_receiver, SIGUSR1);
        } else {
            std::cout << 1;
            kill(pid_receiver, SIGUSR2);
        }
        
        while (!ready);
        ready = false;
    }
    std::cout << std::endl;
    std::cout << "Все биты успешно переданы" << std::endl;
    kill(pid_receiver, SIGINT);

    return 0;
}
