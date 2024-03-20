// server.c
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

int shm_id;
int pipe_fd;

void handler(int sig) {
    printf("server: received SIGINT\n");
    shmctl(shm_id, IPC_RMID, NULL); // remove shared memory segment
    exit(0);
}

int main(){
    int *share;

    signal(SIGINT, handler);

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

    mknod("/tmp/server_pipe", S_IFIFO | 0666, 0);
    pipe_fd = open("/tmp/server_pipe", O_RDONLY | O_NONBLOCK);

    char buffer[5];
    while(1){
        ssize_t count = read(pipe_fd, buffer, 5);
        if (count > 0) {
            if (strcmp(buffer, "exit") == 0) {
                printf("server: received special value from client\n");
                shmctl(shm_id, IPC_RMID, NULL); // remove shared memory segment
                break;
            }
        }
        printf("read a random number %d\n", share[1]);
        sleep(1);
    }
    close(pipe_fd);
    unlink("/tmp/server_pipe");
    return 0;
}