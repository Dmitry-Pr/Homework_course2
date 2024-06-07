#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BASE_FILENAME "a"
#define MAX_FILENAME_LENGTH 1024

int main() {
    int depth = 0;
    char filename[MAX_FILENAME_LENGTH];
    char linkname[MAX_FILENAME_LENGTH];
    int fd;

    // Создаем базовый файл "a"
    fd = open(BASE_FILENAME, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("Ошибка при создании базового файла");
        return 1;
    }
    close(fd);

    snprintf(filename, MAX_FILENAME_LENGTH, BASE_FILENAME);

    while (1) {
        // Создаем имя для новой символьной ссылки
        snprintf(linkname, MAX_FILENAME_LENGTH, "l_%d", depth);

        // Создаем символьную ссылку
        if (symlink(filename, linkname) == -1) {
            perror("Ошибка при создании символьной ссылки");
            break;
        }

        // Пытаемся открыть новую символьную ссылку
        fd = open(linkname, O_RDONLY);
        if (fd == -1) {
            perror("Ошибка при открытии файла");
            break;
        }
        close(fd);

        // Подготавливаемся к следующей итерации
        snprintf(filename, MAX_FILENAME_LENGTH, "%s", linkname);
        depth++;
    }

    printf("Глубина рекурсии для символьных ссылок при открытии файлов: %d\n", depth);

    // Очистка созданных файлов и ссылок
    for (int i = 0; i <= depth; i++) {
        snprintf(linkname, MAX_FILENAME_LENGTH, "l_%d", i);
        unlink(linkname);
    }
    unlink(BASE_FILENAME);

    return 0;
}
