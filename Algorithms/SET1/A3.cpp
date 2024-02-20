#include "iostream"

long long hashFunc(std::string s ) {
    const int p = 31;
    long long hash = 0, p_pow = 1;
    for (size_t i = 0; i < s.length(); ++i) {
        hash += (s[i] - 'a' + 1) * p_pow;
        p_pow *= p;
    }
    return hash;
}

int main() {
    std::string s = "A";
    std::cout << hashFunc("A") << '\n';
    std::cout << hashFunc("a") << '\n';
    std::cout << hashFunc("Z") << '\n';
    std::cout << hashFunc("z") << '\n';
    std::cout << hashFunc("0") << '\n';
    std::cout << hashFunc("9") << '\n';
}