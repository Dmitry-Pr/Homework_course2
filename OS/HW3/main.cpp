#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

int main(int argc, char *argv[]) {
    u_int64_t n;
    int status = 0;
    if (argc == 1) {
        n = 5;
    } else {
        n = std::stoull(argv[1]);
    }
    pid_t pid, ppid, chpid;
    chpid = fork();
    pid = getpid();
    ppid = getppid();

    if (chpid <= -1) {
        printf("Incorrect fork syscall\n");
        exit(1);
    } else if (chpid == 0) { // child
        std::cout << "I am a child" << std::endl
                  << "My pid = " << (int) pid << ", my parent is "
                  << (int) ppid << " and I have no child" << std::endl;
        u_int64_t fact = 1;
        for (u_int64_t j = 1; j <= n; j++) {
            auto prev = fact;
            fact *= j;
            if (fact != 0 && fact / j != prev) {
                std::cout << "Overflow at fact = " << prev << " and next multiplier is " << j << std::endl;
                exit(1);
            }
        }
        std::cout << n << "! " << fact << std::endl;
        exit(0);
    } else { // parent
        std::cout << "I am a parent" << std::endl
                  << "My pid = " << (int) pid << ", my parent is "
                  << (int) ppid << " and my child is " << (int) chpid << std::endl;
        u_int64_t a1 = 1, a2 = 1;
        if (n == 1) {
            std::cout << a1;
        } else if (n == 2) {
            std::cout << a2;
        } else {
            for (u_int64_t i = 3; i <= n; i++) {
                if (a1 > UINT64_MAX - a2) {
                    std::cout << "Overflow at a1 = " << a1 << " and a2 = " << a2
                              << "while computing fibonacci № " << i << std::endl;
                    exit(1);
                }
                auto t = a1 + a2;
                a1 = a2;
                a2 = t;
            }
            std::cout << "fibonacci № " << n << ": " << a2 << std::endl;
        }
        wait(&status);
        execlp("ls", "ls", "-la", NULL);
    }
    return 0;
}
