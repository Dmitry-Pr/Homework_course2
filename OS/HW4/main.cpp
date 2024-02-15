// loop-file-read.c
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <iostream>

const int size = 32;

int conv(std::string s) {
    int res = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        res *= 8;
        res += (s[i] - '0');
    }
    return res;
}

int main(int argc, char *argv[]) {
    const char *in;
    const char *out;
    int status = 0;
    if (argc < 3) {
        in = "input.txt";
        out = "out.txt";
    } else {
        in = argv[1];
        out = argv[2];
    }
    int fd_in, fd_out;
    char buffer[size + 1];
    ssize_t read_bytes;

    if ((fd_in = open(in, O_RDONLY)) < 0) {
        printf("Can\'t open file to read\n");
        exit(-1);
    }
    int flag = 0000;
    int rval;
    rval = access(in, R_OK);
    if (rval == 0) {
        flag += 444;
    }
    rval = access(in, W_OK);
    if (rval == 0) {
        flag += 222;
    }
    rval = access(in, X_OK);
    if (rval == 0) {
        flag += 111;
    }
    std::cout << flag;
    if ((fd_out = open(out, O_RDWR | O_CREAT, conv(std::to_string(flag)))) < 0) {
        printf("Can\'t open file to write\n");
        exit(-1);
    }

    do {
        read_bytes = read(fd_in, buffer, size);
        if (read_bytes == -1) {
            printf("Can\'t write this file\n");
            exit(-1);
        }
        write(fd_out, buffer, read_bytes);
    } while (read_bytes == size);

    if (close(fd_in) < 0) {
        printf("Can\'t close file to read\n");
    }
    if (close(fd_out) < 0) {
        printf("Can\'t close file to write\n");
    }

    return 0;
}
