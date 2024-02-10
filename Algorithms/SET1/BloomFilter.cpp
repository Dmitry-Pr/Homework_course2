#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

class BloomFilter {
public:
    BloomFilter(size_t hashes, size_t bits);

    ~BloomFilter();

    void add(const std::string &obj);

    bool verify(const std::string &obj);

    double getFPRate() const;

    size_t numberOfHashFunctions() const;

    size_t numberOfBits() const;

private:
    size_t hashFuncN, bitsN, FPCount = 0, verifyCount = 0;
    std::unordered_set<std::string> *stringStorage;
    std::vector<bool> *filter;
};


BloomFilter::BloomFilter(size_t hashes, size_t bits) {
    hashFuncN = hashes;
    bitsN = bits;
    filter = new std::vector<bool>(bitsN * hashFuncN, false);
    stringStorage = new std::unordered_set<std::string>();
}

BloomFilter::~BloomFilter() {
    delete filter;
    delete stringStorage;
}

void BloomFilter::add(const std::string &obj) {
    if (hashFuncN == 0 || bitsN == 0) {
        return;
    }
    stringStorage->insert(obj);
    auto h = std::hash<std::string>{}(obj);
    (*filter)[h % bitsN] = true;
    for (size_t i = 1; i < hashFuncN; i++) {
        h = std::hash<std::string>{}(obj + std::to_string(i));
        (*filter)[i * bitsN + h % bitsN] = true;
    }
}

bool BloomFilter::verify(const std::string &obj) {
    if (hashFuncN == 0 || bitsN == 0) {
        return false;
    }
    verifyCount++;
    auto h = std::hash<std::string>{}(obj);
    bool flag = true;
    flag &= (*filter)[h % bitsN];
    for (size_t i = 1; i < hashFuncN && flag; i++) {
        h = std::hash<std::string>{}(obj + std::to_string(i));
        flag &= (*filter)[i * bitsN + h % bitsN];
    }
    if (flag && (*stringStorage).count(obj) == 0) {
        FPCount++;
    }
    return flag;
}

double BloomFilter::getFPRate() const {
    if (FPCount == 0) {
        return 0;
    }
    return static_cast<double>(FPCount) / static_cast<double>(verifyCount);
}

size_t BloomFilter::numberOfHashFunctions() const {
    return hashFuncN;
}

size_t BloomFilter::numberOfBits() const {
    return bitsN;
}
//int main() {
//    BloomFilter filter = BloomFilter(3, 8);
//    filter.add("123");
//    std::cout << filter.getFPRate() << '\n' << filter.numberOfBits() << '\n' << filter.numberOfHashFunctions() << '\n';
//    filter.add("321");
//    std::cout << filter.verify("123") << '\n';
//    std::cout << filter.getFPRate() << '\n';
//
//}