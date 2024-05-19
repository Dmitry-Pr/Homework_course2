#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <arpa/inet.h>

const int NUM_DEPARTMENTS = 3;
std::vector<int> departments(NUM_DEPARTMENTS, 0);

void handle_customer(int department) {
    // Обработка запроса покупателя
}

int main(int argc, char *argv[]) {
    // Создание сокета, привязка к адресу и порту, перевод в режим прослушивания
    // В цикле принимаем запросы от покупателей и распределяем их по продавцам
    // После обработки запроса продавцом, отправляем сообщение обратно покупателю
}