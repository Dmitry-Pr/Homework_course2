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

void handler(int sig) {
    printf("client: received SIGINT\n");
    kill(share[0], SIGUSR1); // send signal to server before exit
    exit(0);
}

int main(){
    int shm_fd;
    int num;
    signal(SIGINT, handler);
    srand(time(NULL));
    shm_fd = shm_open("/myshm", O_CREAT | O_RDWR, 0666);
    printf("shm_fd = %d\n", shm_fd);
    if(shm_fd < 0){
        perror("shm_open()");
        exit(1);
    }

    ftruncate(shm_fd, getpagesize());

    share = (int *)mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(share == MAP_FAILED){
        perror("mmap()");
        exit(2);
    }
    printf("share = %p\n", share);

    printf("Server's PID = %d\n", share[0]);

    while(1){
        num = random() % 1000;
        share[1] = num; // write to the second integer in shared memory
        printf("write a random number %d\n", num);
        sleep(1);
    }
    return 0;
}