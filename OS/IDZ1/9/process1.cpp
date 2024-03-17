#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 5

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(-1);
    }
    char buffer[BUFFER_SIZE];
    char zero[BUFFER_SIZE] = {0};
    int letterCounts[26] = {0};
    const char *fifo1 = "/tmp/fifo1";
    const char *fifo2 = "/tmp/fifo2";
    mknod(fifo1, S_IFIFO | 0666, 0);
    mknod(fifo2, S_IFIFO | 0666, 0);
    int size;
    int fd;
    int fd_input = open(argv[1], O_RDONLY);
    if (fd_input < 0) {
        printf("Can\'t open file for reading\n");
        exit(-1);
    }
    do {

        size = read(fd_input, buffer, BUFFER_SIZE);
        if (size <= 0) {
            break;
        }
        fd = open(fifo1, O_WRONLY);
        if (write(fd, buffer, size) != size) {
            printf("Can't write to fifo1\n");
            exit(-1);
        }
        close(fd);
    } while (1);
    close(fd_input);
    fd = open(fifo1, O_WRONLY);
    if (write(fd, zero, BUFFER_SIZE) != BUFFER_SIZE) {
        printf("Can't write to fifo1\n");
        exit(-1);
    }
    close(fd);
    fd = open(fifo2, O_RDONLY);
    if (read(fd, letterCounts, sizeof(letterCounts)) != sizeof(letterCounts)) {
        printf("Can't read from fifo2\n");
        exit(-1);
    }
    close(fd);
    fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        printf("Can't open file for writing\n");
        exit(-1);
    }
    char out_buffer[50];
    for (int i = 0; i < 26; i++) {
        int len = sprintf(out_buffer, "%c: %d\n", 'A' + i, letterCounts[i]);
        if (write(fd, out_buffer, len) != len) {
            printf("Can't write to output file\n");
            exit(-1);
        }
    }
    close(fd);
    printf("End of process 1 work\n");
    unlink(fifo1);
    unlink(fifo2);
    return 0;
}