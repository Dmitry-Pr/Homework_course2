// server.c
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

int shm_fd;

void handler(int sig) {
    printf("server: received SIGINT\n");
    shm_unlink("/myshm"); // remove shared memory segment
    exit(0);
}

int main() {
    int *share;

    signal(SIGINT, handler);

    shm_fd = shm_open("/myshm", O_CREAT | O_RDWR, 0666);
    printf("shm_fd = %d\n", shm_fd);
    if (shm_fd < 0) {
        perror("shm_open()");
        exit(1);
    }

    ftruncate(shm_fd, getpagesize());

    share = (int *) mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (share == MAP_FAILED) {
        perror("mmap()");
        exit(2);
    }
    printf("share = %p\n", share);

    share[0] = 0;

    while (1) {
        if (share[0] == 1) {
            printf("server: received special value from client\n");
            shm_unlink("/myshm"); // remove shared memory segment
            break;
        }
        printf("read a random number %d\n", share[1]);
        sleep(1);
    }

    return 0;
}