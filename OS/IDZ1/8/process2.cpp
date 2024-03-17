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

int main() {
    char buffer[BUFFER_SIZE];
    int letterCounts[26] = {0};
    const char *fifo1 = "/tmp/fifo1";
    const char *fifo2 = "/tmp/fifo2";

    int fd = open(fifo1, O_RDONLY);
    read(fd, buffer, BUFFER_SIZE);
    close(fd);
    count_uppercase_letters(buffer, letterCounts);
    fd = open(fifo2, O_WRONLY);
    write(fd, letterCounts, sizeof(letterCounts));
    close(fd);
    printf("End of process 2 work\n");

    return 0;
}