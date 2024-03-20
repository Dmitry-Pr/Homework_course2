// server.c
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>

int shm_id;

void handler(int sig) {
    printf("server: received SIGUSR1\n");
    shmctl(shm_id, IPC_RMID, NULL); // remove shared memory segment
    exit(0);
}

int main(){
    int *share;

    signal(SIGUSR1, handler);

    shm_id = shmget (0x2FF, getpagesize(), 0666 | IPC_CREAT);
    printf("shm_id = %d\n", shm_id);
    if(shm_id < 0){
        perror("shmget()");
        exit(1);
    }

    share = (int *)shmat(shm_id, 0, 0);
    if(share == NULL){
        perror("shmat()");
        exit(2);
    }
    printf("share = %p\n", share);

    share[0] = getpid(); // write server's PID to shared memory

    while(1){
        printf("read a random number %d\n", share[1]);
        sleep(1);
    }

    return 0;
}