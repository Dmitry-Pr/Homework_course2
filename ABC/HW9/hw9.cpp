#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <queue>

pthread_mutex_t rw;
pthread_cond_t cond;
std::queue<int> buf;
int generated = 0;

void *generator(void *num) {
    int pNum = *((int *) num);
    srand(pNum);
    int n = rand();
    sleep(n % 7 + 1);
    n = n % 20 + 1;
    pthread_mutex_lock(&rw);
    buf.push(n);
    std::cout << "Поток " << pNum << " записал число в буфер: " << n << '\n';
    if (buf.size() >= 2) {
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&rw);
    return nullptr;
}

void *sumator(void *arg1) {
    auto n = ((int*)arg1)[0];
    auto m = ((int*)arg1)[1];
    sleep(rand() % 4 + 3);
    pthread_mutex_lock(&rw);
    std::cout << "Найдена сумма: " << n << " + " << m << " = " << n + m << "\n";
    buf.push(n + m);
    generated++;
    if (buf.size() >= 2 || generated == 19) {
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&rw);
    return nullptr;
}

void *conveyor(void *arg) {
    while (true) {
        pthread_mutex_lock(&rw);
        while (buf.size() < 2 && generated != 19) {
            pthread_cond_wait(&cond, &rw);
        }
        if (generated == 19) {
            return (void*) &buf.front();
        }
        auto n = buf.front();
        buf.pop();
        auto m = buf.front();
        buf.pop();
        pthread_mutex_unlock(&rw);
        pthread_t sum;
        int numbers[2] = {n, m};
        pthread_create(&sum, nullptr, sumator, (void*) numbers);
    }
}

int main() {
    pthread_mutex_init(&rw, nullptr);
    pthread_cond_init(&cond, nullptr);
    pthread_t threadP[20];
    int producers[20];
    for (int i = 0; i < 20; i++) {
        producers[i] = i + 1;
        pthread_create(&threadP[i], nullptr, generator, (void *) (producers + i));
    }
    pthread_t conv;
    pthread_create(&conv, nullptr, conveyor, nullptr);
    int* res;
    pthread_join(conv, (void**)&res);
    std::cout << "Итоговая сумма: " << *res << '\n';
}