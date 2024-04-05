// server.c
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

int shm_fd;

void handler(int sig) {
    printf("server: received SIGUSR1\n");
    shm_unlink("/myshm"); // remove shared memory segment
    exit(0);
}

int main(){
    int *share;

    signal(SIGUSR1, handler);
    signal(SIGINT, handler);

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

    share[0] = getpid(); // write server's PID to shared memory

    while(1){
        printf("read a random number %d\n", share[1]);
        sleep(1);
    }

    return 0;
}