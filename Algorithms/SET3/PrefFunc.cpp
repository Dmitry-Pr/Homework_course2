//#include <iostream>
//#include <vector>
//
//std::vector<size_t> pref;
//
//void prefFunc(std::string pattern) {
//    pref.resize(pattern.size());
//    pref[0] = 0;
//    size_t i = 1;
//    size_t j = 0;
//    while (i < pattern.size()) {
//        if (pattern[i] == pattern[j]) {
//            pref[i] = j + 1;
//            j++;
//            i++;
//        } else if (j > 0) {
//            j = pref[j - 1];
//        } else {
//            pref[i] = 0;
//            i++;
//        }
//    }
//}
//
//int main() {
//    std::string pattern;
//    std::cin >> pattern;
//    prefFunc(pattern);
//    for (size_t i: pref) {
//        std::cout << i << " ";
//    }
//}
