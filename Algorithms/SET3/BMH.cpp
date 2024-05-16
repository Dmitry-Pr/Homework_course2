//#include <vector>
//#include <string>
//#include <iostream>
//
//std::vector<size_t> buildShiftTable(const std::string& pattern) {
//    const int ALPHABET_SIZE = 256;
//    std::vector<size_t> table(ALPHABET_SIZE, pattern.size());
//
//    for (size_t i = 0; i < pattern.size() - 1; ++i) {
//        table[pattern[i]] = pattern.size() - 1 - i;
//    }
////    table[pattern.back()] = pattern.size();
//
//    return table;
//}
//
//std::vector<size_t> BMHSearch(const std::string& text, const std::string& pattern) {
//    std::vector<size_t> shiftTable = buildShiftTable(pattern);
//    size_t m = pattern.size();
//    size_t n = text.size();
//    size_t i = m - 1;
//    size_t j = m - 1;
//    std::vector<size_t> ans;
//
//    while (i < n) {
//        if (text[i] == pattern[j]) {
//            if (j == 0) {
//                ans.emplace_back(i);
//                i += m;
//                j = m - 1;
//                continue;
//            }
//            --i;
//            --j;
//        } else {
//            i += m - 1 - j;
//            i += shiftTable[text[i]];
//            j = m - 1;
//        }
//    }
//    return ans;
//}
//int main() {
//    std::ios_base::sync_with_stdio(false);
//    std::cin.tie(NULL);
//    std::string pattern;
//    std::cin >> pattern;
//    std::string text;
//    std::cin >> text;
//    std::vector<size_t> ans = BMHSearch(text, pattern);
//    std::cout << ans.size() << '\n';
//    for (size_t i: ans) {
//        std::cout << i << '\n';
//    }
////    ans = buildShiftTable(pattern);
////    for (size_t i: ans) {
////        if (i != pattern.size())
////        std::cout << i << " ";
////    }
//}