#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int count_uppercase_letters(char *str, int *count, int size) {
    // Проверяем корректность аргументов
    if (str == NULL || count == NULL) {
        return -1;
    }

    // Перебираем символы строки
    for (int i = 0; i < size; i++) {
        // Проверяем, является ли символ прописной буквой
        if (str[i] >= 'A' && str[i] <= 'Z') {
            // Увеличиваем счетчик для этой буквы
            count[str[i] - 'A']++;
        }
    }

    return 0;
}

#define BUFFER_SIZE 5

int main() {
    char buffer[BUFFER_SIZE];
    int letterCounts[26] = {0};
    const char *fifo1 = "/tmp/fifo1";
    const char *fifo2 = "/tmp/fifo2";
    int size;
    int fd;
    do {
        fd = open(fifo1, O_RDONLY);
        size = read(fd, buffer, BUFFER_SIZE);

        close(fd);
        if (size <= 0) {
            break;
        }
        count_uppercase_letters(buffer, letterCounts, size);
    } while (buffer[0] != '\0');
    fd = open(fifo2, O_WRONLY);
    if (write(fd, letterCounts, sizeof(letterCounts)) != sizeof(letterCounts)) {
        printf("Can't write to fifo2\n");
        exit(-1);
    }
    close(fd);
    printf("End of process 2 work\n");
    return 0;
}