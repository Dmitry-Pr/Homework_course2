//#include <iostream>
//#include <vector>
//
//std::vector<size_t> pref;
//
//void prefFuncSpecified(std::string pattern) {
//    pref.resize(pattern.size());
//    pref[0] = 0;
//    size_t i = 1;
//    size_t j = 0;
//    while (i < pattern.size()) {
//        if (pattern[i] == pattern[j]) {
//            pref[i] = j + 1;
//            pref[i - 1] = 0;
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
//    std::vector<size_t> ans;
//    std::string pattern;
//
//    std::cin >> pattern;
//    std::string text;
//    std::cin >> text;
//    prefFuncSpecified(pattern);
//    size_t txtIndex = 0;
//    size_t patIndex = 0;
//    size_t n = text.size();
//    size_t m = pattern.size();
//    while (txtIndex < n) {
//        if (text[txtIndex] == pattern[patIndex]) {
//            if (patIndex == m - 1) {
//                ans.push_back(txtIndex - m + 1);
//                txtIndex++;
//                patIndex = pref[patIndex];
//            } else {
//                txtIndex++;
//                patIndex++;
//            }
//        } else {
//            if (patIndex > 0) {
//                patIndex = pref[patIndex - 1];
//            } else {
//                txtIndex++;
//            }
//        }
//    }
//    std::cout << ans.size() << std::endl;
//    for (size_t i: ans) {
//        std::cout << i << std::endl;
//    }
//}