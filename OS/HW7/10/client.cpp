// client.c
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>

int *share;
int shm_fd;

void handler(int sig) {
    printf("client: received SIGINT\n");
    share[0] = 1; // write special value to shared memory
    exit(0);
}

int main() {
    int num;
    signal(SIGINT, handler);
    srand(time(NULL));
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

    while (1) {
        if (share[0] == 1) {
            printf("client: received special value from server\n");
            break;
        }
        num = random() % 1000;
        share[1] = num; // write to the second integer in shared memory
        printf("write a random number %d\n", num);
        sleep(1);
    }
    return 0;
}