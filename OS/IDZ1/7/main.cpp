#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

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
    const char *fifo1 = "/tmp/fifo1";
    const char *fifo2 = "/tmp/fifo2";
    mknod(fifo1, S_IFIFO | 0666, 0);
    mknod(fifo2, S_IFIFO | 0666, 0);

    // Создаем два дочерних процесса
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
        fd = open(fifo1, O_WRONLY);
        write(fd, buffer, BUFFER_SIZE);
        close(fd);
        fd = open(fifo2, O_RDONLY);
        read(fd, letterCounts, sizeof(letterCounts));
        close(fd);
        fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
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
        printf("End of process 1 work\n");
    } else { // process 2
        int fd = open(fifo1, O_RDONLY);
        int size = read(fd, buffer, BUFFER_SIZE);
        close(fd);
        count_uppercase_letters(buffer, letterCounts);
        fd = open(fifo2, O_WRONLY);
        write(fd, letterCounts, sizeof(letterCounts));
        close(fd);
        printf("End of process 2 work\n");
    }
    unlink(fifo1);
    unlink(fifo2);
}