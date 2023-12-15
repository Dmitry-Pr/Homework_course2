#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fstream>

pthread_mutex_t mutex;
sem_t canFly;
sem_t canEat;
sem_t notFull;
int hive = 0;
int beesInHive;
int numberOfBees = 0;
std::ofstream out;


void *bee(void *arg) {
    int beeNum = *((int *) arg);
    while (true) {
        sem_wait(&notFull); // if hive is full, don't fly
        sem_wait(&canFly); // if it is no bees in hive, don't fly
        pthread_mutex_lock(&mutex);
        beesInHive--;
        std::cout << "Пчела " << beeNum << " улетела. Теперь в улье " << beesInHive << " пчел\n";
        if (out.is_open()) {
            out << "Пчела " << beeNum << " улетела. Теперь в улье " << beesInHive << " пчел\n";
        }
        pthread_mutex_unlock(&mutex);
        srand(beeNum);
        sleep(rand() % 5 + 1);
        pthread_mutex_lock(&mutex);
        hive++;
        beesInHive++;
        std::cout << "Пчела " << beeNum << " принесла в улей мед, теперь в нем " << hive << '\n';
        if (out.is_open()) {
            out << "Пчела " << beeNum << " принесла в улей мед, теперь в нем " << hive << '\n';
        }
        if (hive == 15) {
            sem_post(&canEat); // bear can eat
        }
        sem_post(&canFly); // the bee has returned
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 2 + 1);
    }

}

void *bear(void *arg) {
    while (true) {
        sem_wait(&canEat);
        pthread_mutex_lock(&mutex);
        if (hive == 30) {
            std::cout << "Медведь проиграл, пчелы запаслись медом...\n";
            if (out.is_open()) {
                out << "Медведь проиграл, пчелы запаслись медом...\n";
            }
            return nullptr;
        }
        if (beesInHive < 3) {
            std::cout << "Медведь все съел, теперь в улье: " << 0 << '\n';
            if (out.is_open()) {
                out << "Медведь все съел, теперь в улье: " << 0 << '\n';
            }
            for (int i = 0; i < hive; i++) {
                sem_post(&notFull); // hive is empty
            }
            hive = 0;
        } else {
            std::cout << "Медведя покусали, в улье было " << beesInHive << " пчел" << '\n';
            if (out.is_open()) {
                out << "Медведя покусали, в улье было " << beesInHive << " пчел" << '\n';
            }
            sem_post(&canEat); // if didn't eat, try again later
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

int main(int argc, char *argv[]) {
//    std::cout << "Введите количество пчел в улье. Должно быть больше 3\n";
//    std::cin >> numberOfBees;'
    pthread_mutex_init(&mutex, nullptr);
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-in") == 0) {
            std::ifstream inFile(argv[++i]);
            if (inFile.is_open()) {
                inFile >> numberOfBees;
            }
        } else if (strcmp(argv[i], "-out") == 0) {
            out.open(argv[++i]);
        } else if (strcmp(argv[i], "-bees") == 0) {
            numberOfBees = std::stoi(argv[++i]);
        }
    }
    if (numberOfBees < 4) {
        srand(time(NULL));
        numberOfBees = rand() % 30 + 4;
        pthread_mutex_lock(&mutex);
        std::cout << "Пчел не может быть меньше 4. Будем считать, что в улье " << numberOfBees << " пчелы\n";
        pthread_mutex_unlock(&mutex);
    }
    beesInHive = numberOfBees;

    sem_init(&canFly, 0, numberOfBees - 1);
    sem_init(&notFull, 0, 30);
    sem_init(&canEat, 0, 0);
    pthread_t bees[numberOfBees];
    int beeNumbers[numberOfBees];
    for (int i = 0; i < numberOfBees; i++) {
        beeNumbers[i] = i + 1;
        pthread_create(&bees[i], nullptr, bee, (void *) (beeNumbers + i));
    }
    pthread_t bearT;
    pthread_create(&bearT, nullptr, bear, nullptr);
    int *res;
    pthread_join(bearT, (void **) &res);

}