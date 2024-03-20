// server.c
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>

int shm_id;

void handler(int sig) {
    printf("server: received SIGINT\n");
    shmctl(shm_id, IPC_RMID, NULL); // remove shared memory segment
    exit(0);
}

int main() {
    int *share;

    signal(SIGINT, handler);

    shm_id = shmget(0x2FF, getpagesize(), 0666 | IPC_CREAT);
    printf("shm_id = %d\n", shm_id);
    if (shm_id < 0) {
        perror("shmget()");
        exit(1);
    }

    share = (int *) shmat(shm_id, 0, 0);
    if (share == NULL) {
        perror("shmat()");
        exit(2);
    }
    printf("share = %p\n", share);

    share[0] = 0;

    while (1) {
        if (share[0] == 1) {
            printf("server: received special value from client\n");
            shmctl(shm_id, IPC_RMID, NULL); // remove shared memory segment
            break;
        }
        printf("read a random number %d\n", share[1]);
        sleep(1);
    }

    return 0;
}