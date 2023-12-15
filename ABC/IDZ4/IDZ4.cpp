#include <pthread.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fstream>

pthread_mutex_t mutex;
pthread_cond_t flyCond;
pthread_cond_t canEat;
int hive = 0; // amount of honey
int beesInHive; // number of bees in hive
int numberOfBees = 0; // input number of bees
std::ofstream out;


void *bee(void *arg) {
    int beeNum = *((int *) arg);
    while (true) {
        pthread_mutex_lock(&mutex);
        while (beesInHive == 1 || hive + numberOfBees - beesInHive >= 30) { // is it possible to fly from hive
            pthread_cond_wait(&flyCond, &mutex);    // wait if not possible
        }
        beesInHive--;
        std::cout << "Пчела " << beeNum << " улетела. Теперь в улье " << beesInHive << " пчел\n";
        if (out.is_open()) {
            out << "Пчела " << beeNum << " улетела. Теперь в улье " << beesInHive << " пчел\n";
        }
        pthread_mutex_unlock(&mutex);
        srand(beeNum);
        sleep(rand() % 5 + 1); // from 1 to 5
        pthread_cond_broadcast(&flyCond); // the bee has returned
        pthread_mutex_lock(&mutex);
        hive++;
        if (hive >= 15) {
            pthread_cond_broadcast(&canEat); // the bear can eat
        }
        beesInHive++;
        std::cout << "Пчела " << beeNum << " принесла в улей мед, теперь в нем " << hive << '\n';
        if (out.is_open()) {
            out << "Пчела " << beeNum << " принесла в улей мед, теперь в нем " << hive << '\n';
        }
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 2 + 1); // from 1 to 2
    }

}

void *bear(void *arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        while (hive < 15) {
            pthread_cond_wait(&canEat, &mutex); // wait if can't eat
        }
        if (hive == 30) {   // hive is full, it is always > 2 bees in there
            std::cout << "Медведь проиграл, пчелы запаслись медом...\n";
            if (out.is_open()) {
                out << "Медведь проиграл, пчелы запаслись медом...\n";
            }
            return nullptr;
        }
        if (beesInHive < 3) {
            hive = 0;
            std::cout << "Медведь все съел, теперь в улье: " << hive << '\n';
            if (out.is_open()) {
                out << "Медведь все съел, теперь в улье: " << hive << '\n';
            }
        } else {
            std::cout << "Медведя покусали, в улье было " << beesInHive << " пчел" << '\n';
            if (out.is_open()) {
                out << "Медведя покусали, в улье было " << beesInHive << " пчел" << '\n';
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(1); // sleeps if have eaten, recovers if is beaten
    }
}

int main(int argc, char *argv[]) {
//    std::cout << "Введите количество пчел в улье. Должно быть больше 3\n";
//    std::cin >> numberOfBees;'
    for (int i = 1; i < argc; i++) { // input from terminal with keys -in, -out, -bees
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
        numberOfBees = rand() % 30 + 4; // from 4 (minimum) to 33 (can be changed)
        std::cout << "Пчел не может быть меньше 4. Будем считать, что в улье " << numberOfBees << " пчелы\n";

    }
    beesInHive = numberOfBees;
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&flyCond, nullptr);
    pthread_cond_init(&canEat, nullptr);
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