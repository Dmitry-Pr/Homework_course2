#include "pthread.h"
#include "iostream"
#include <iomanip>
#include <ctime>
#include <chrono>

const uint64_t n = 450000000;
const int threadNumber = 1000; // 1
double *A;
double *B;

void *func(void *param) {
    uint64_t shift = n / threadNumber;
    uint64_t p = (*(uint64_t *) param) * shift;
    auto sum = new double;
    *sum = 0;
    for (uint64_t i = p; i < p + shift; i++) {
        *sum += A[i] * B[i];
    }
    return (void *) sum;
}


int main() {
    A = new double[n];
    B = new double[n];
    double rez = 0.0;
    for (int i = 0; i < n; i++) {
        A[i] = i + 1;
        B[i] = n - i;
    }
    pthread_t thread[threadNumber];
    uint64_t number[threadNumber];

    auto start = std::chrono::high_resolution_clock::now();
    if (threadNumber == 1) {
        number[0] = 0;
        rez = *(double *) func((void *) number);
    } else {

        for (uint64_t i = 0; i < threadNumber; i++) {
            number[i] = i;
            pthread_create(&thread[i], nullptr, func, (void *) (number + i));
        }


        double *sum;
        for (uint64_t i = 0; i < threadNumber; i++) {
            pthread_join(thread[i], (void **) &sum);
            rez += *sum;
            delete sum;
        }
    }

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long millisec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "Векторное произведение = " << std::setprecision(20) << rez << "\n";
    std::cout << "Размер векторов " << n << '\n';
    std::cout << "Число потоков " << threadNumber << '\n';
    std::cout << "Общее время " << millisec << '\n';
}