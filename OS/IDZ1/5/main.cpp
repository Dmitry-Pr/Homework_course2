#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int count_uppercase_letters(char *str, int *count) {
    // Проверяем корректность аргументов
    if (str == NULL || count == NULL) {
        return -1;
    }

    // Перебираем символы строки
    for (int i = 0; str[i] != '\0'; i++) {
        // Проверяем, является ли символ прописной буквой
        if (str[i] >= 'A' && str[i] <= 'Z') {
            // Увеличиваем счетчик для этой буквы
            count[str[i] - 'A']++;
        }
    }

    return 0;
}

#define BUFFER_SIZE 5000

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(-1);
    }
    char buffer[BUFFER_SIZE];
    int letterCounts[26] = {0};

    // Создаем именованные каналы
    const char *fifo1 = "/tmp/fifo1";
    const char *fifo2 = "/tmp/fifo2";
    mknod(fifo1, S_IFIFO | 0666, 0);
    mknod(fifo2, S_IFIFO | 0666, 0);

    // Создаем три дочерних процесса
    pid_t pid1 = fork();
    if (pid1 < 0) {
        exit(-1);
    }
    if (pid1 > 0) { // process 1
        int fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            printf("Can\'t open file for reading\n");
            exit(-1);
        }
        read(fd, buffer, BUFFER_SIZE);
        close(fd);
        int fd1 = open(fifo1, O_WRONLY);
        write(fd1, buffer, BUFFER_SIZE);
        close(fd1);
        printf("End of process 1 work\n");
    } else {
        pid_t pid2 = fork();
        if (pid2 < 0) {
            exit(-1);
        }
        if (pid2 == 0) { // process 2
            int fd1 = open(fifo1, O_RDONLY);
            read(fd1, buffer, BUFFER_SIZE);
            close(fd1);
            count_uppercase_letters(buffer, letterCounts);
            int fd2 = open(fifo2, O_WRONLY);
            write(fd2, letterCounts, sizeof(letterCounts));
            close(fd2);
            printf("End of process 2 work\n");
        } else {
            // process 3
            int fd2 = open(fifo2, O_RDONLY);
            read(fd2, letterCounts, sizeof(letterCounts));
            close(fd2);
            int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd < 0) {
                printf("Can't open file for writing\n");
                exit(-1);
            }
            char buffer[50];
            for (int i = 0; i < 26; i++) {
                int len = sprintf(buffer, "%c: %d\n", 'A' + i, letterCounts[i]);
                write(fd, buffer, len);
            }
            close(fd);
            printf("End of process 3 work\n");
        }
    }

    // Удаляем именованные каналы
    unlink(fifo1);
    unlink(fifo2);
}