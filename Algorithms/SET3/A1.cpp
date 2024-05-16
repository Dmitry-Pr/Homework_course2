#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <iostream>

class StringGenerator {
private:
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-";
public:
    StringGenerator() {
        std::random_device rd;
        gen = std::mt19937(rd());
        dist = std::uniform_int_distribution<>(0, alphabet.size() - 1);
    }

    std::string generateString(int length) {
        std::string result;
        for (int i = 0; i < length; ++i) {
            result += alphabet[dist(gen)];
        }
        return result;
    }

    std::vector<std::string> generateArray(int size, int minLen, int maxLen) {
        std::vector<std::string> result;
        std::uniform_int_distribution<> lenDist(minLen, maxLen);
        for (int i = 0; i < size; ++i) {
            result.push_back(generateString(lenDist(gen)));
        }
        return result;
    }

    std::vector<std::string> generateSortedArray(int size, int minLen, int maxLen) {
        auto result = generateArray(size, minLen, maxLen);
        std::sort(result.begin(), result.end());
        return result;
    }

    std::vector<std::string> generateReverseSortedArray(int size, int minLen, int maxLen) {
        auto result = generateSortedArray(size, minLen, maxLen);
        std::reverse(result.begin(), result.end());
        return result;
    }

    std::vector<std::string> generateNearlySortedArray(int size, int minLen, int maxLen, int swapCount) {
        auto result = generateSortedArray(size, minLen, maxLen);
        std::uniform_int_distribution<> indexDist(0, size - 1);
        for (int i = 0; i < swapCount; ++i) {
            int index1 = indexDist(gen);
            int index2 = indexDist(gen);
            std::swap(result[index1], result[index2]);
        }
        return result;
    }

    std::vector<std::string> getSubArray(const std::vector<std::string> &array, size_t start, long long size) {
        std::vector<std::string> v(array.begin(), array.begin() + size);
        return v;
    }


};

class Sort {
private:
    int partition(std::vector<std::string> &a, int left, int right, size_t &sum) {
        srand(time(NULL));
        int random = left + rand() % (right - left);
        std::swap(a[random], a[right]);
        std::string pivot = a[right];
        int pIndex = left;
        for (int i = left; i < right; i++) {
            if (a[i] <= pivot) {
                std::swap(a[i], a[pIndex]);
                pIndex++;
            }
            sum += std::min(a[i].size(), pivot.size());
        }
        std::swap(a[pIndex], a[right]);
        return pIndex;
    }

public:
    void quickSort(std::vector<std::string> &a, int left, int right, size_t &sum) {
        if (left >= right) {
            return;
        }
        auto pivot = partition(a, left, right, sum);
        quickSort(a, left, pivot - 1, sum);
        quickSort(a, pivot + 1, right, sum);
    }

    void mergeSort(std::vector<std::string> &a, int left, int right, size_t &sum) {
        if (left == right) {
            return;
        }
        auto mid = left + (right - left) / 2;
        mergeSort(a, left, mid, sum);
        mergeSort(a, mid + 1, right, sum);
        int i = left;
        int j = mid + 1;
        std::vector<std::string> b;
        while (i <= mid && j <= right) {
            if (a[i] < a[j]) {
                b.push_back(a[i++]);
            } else {
                b.push_back(a[j++]);
            }
            sum += std::min(a[i].size(), a[j].size());
        }
        while (i <= mid) {
            b.push_back(a[i++]);
        }
        while (j <= right) {
            b.push_back(a[j++]);
        }
        for (int k = left; k <= right; k++) {
            a[k] = b[k - left];
        }
    }

    std::vector<std::string> ternaryQuickSort(std::vector<std::string> &r, size_t l, size_t &sum) {
        if (r.size() <= 1) return r;
        std::vector<std::string> rExcl;
        std::vector<std::string> tmp;
        for (size_t i = 0; i < r.size(); i++) {
            if (r[i].size() == l) {
                rExcl.push_back(r[i]);
            } else {
                tmp.push_back(r[i]);
            }
        }
        r = tmp;
        srand(time(NULL));
        int random = rand() % r.size();
        std::swap(r[random], r[r.size() - 1]);
        std::string pivot = r[r.size() - 1];
        auto rLess = std::vector<std::string>();
        auto rGreater = std::vector<std::string>();
        auto rEqual = std::vector<std::string>();
        for (size_t i = 0; i < r.size(); i++) {
            if (r[i][l] < pivot[l]) {
                rLess.push_back(r[i]);
                sum++;
            } else if (r[i][l] > pivot[l]) {
                sum += 2;
                rGreater.push_back(r[i]);
            } else {
                sum += 2;
                rEqual.push_back(r[i]);
            }
        }
        rLess = ternaryQuickSort(rLess, l, sum);
        rGreater = ternaryQuickSort(rGreater, l, sum);
        rEqual = ternaryQuickSort(rEqual, l + 1, sum);
        std::vector<std::string> result;
        std::copy(rExcl.begin(), rExcl.end(), std::back_inserter(result));
        std::copy(rLess.begin(), rLess.end(), std::back_inserter(result));
        std::copy(rEqual.begin(), rEqual.end(), std::back_inserter(result));
        std::copy(rGreater.begin(), rGreater.end(), std::back_inserter(result));
        return result;
    }
};

int main() {
    StringGenerator sg;
    Sort sort;
    auto randomArr = sg.generateArray(3000, 10, 200);

    std::vector<std::string> sortedArr(randomArr.size());
    std::copy(randomArr.begin(), randomArr.end(), sortedArr.begin());
    std::sort(sortedArr.begin(), sortedArr.end());
    size_t sum = 0;
    for (const auto &s: sg.getSubArray(sortedArr, 0, 3)) {
        std::cout << s << std::endl;
    }
    std::cout << sum << std::endl;

    auto reversedArr = sg.generateReverseSortedArray(3000, 10, 200);
    auto nearlySortedArr = sg.generateNearlySortedArray(3000, 10, 200, 300);
    sum = 0;
    auto res = sort.ternaryQuickSort(randomArr, 0, sum);
    for (const auto &s: sg.getSubArray(res, 0, 3)) {
        std::cout << s << std::endl;
    }
    std::cout << sum << std::endl;


}