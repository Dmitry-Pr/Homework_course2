// client.c
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>

int *share;

void handler(int sig) {
    printf("client: received SIGINT\n");
    share[0] = 1; // write special value to shared memory
    exit(0);
}


int main() {
    int shm_id;
    int num;
    signal(SIGINT, handler);
    srand(time(NULL));
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