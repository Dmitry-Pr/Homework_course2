#!/bin/bash

# Создаем начальный файл
touch a

# Инициализируем переменные
prev_link="a"
depth=0

# Функция для очистки созданных файлов при завершении
cleanup() {
    for ((i=0; i<=depth; i++)); do
        rm -f "link_$i"
    done
    rm -f a
}
trap cleanup EXIT

# Бесконечный цикл для создания символических ссылок и проверки
while true; do
    new_link="link_$depth"
    ln -s "$prev_link" "$new_link"

    # Попытка открыть новый символический файл
    if ! cat "$new_link" > /dev/null 2>&1; then
        echo "Глубина рекурсии: $depth"
        exit 0
    fi

    # Обновляем переменные для следующей итерации
    prev_link="$new_link"
    ((depth++))
done
