//#include <string>
//#include <vector>
//#include <iostream>
//
//size_t levenshteinDistance(const std::string &s1, const std::string &s2) {
//    size_t M = s1.size();
//    size_t N = s2.size();
//    std::vector<size_t> previousRow(N + 1, 0), currentRow(N + 1, 0);
//
//    for (size_t i = 0; i <= N; ++i) {
//        previousRow[i] = i;
//    }
//
//    for (size_t i = 1; i <= M; ++i) {
//        currentRow[0] = i;
//        for (size_t j = 1; j <= N; ++j) {
//            if (s1[i - 1] == s2[j - 1]) {
//                currentRow[j] = previousRow[j - 1];
//            } else {
//                currentRow[j] = std::min(previousRow[j - 1], std::min(previousRow[j], currentRow[j - 1])) + 1;
//            }
//        }
//        previousRow = currentRow;
//    }
//
//    return previousRow[N];
//}
//
//int main() {
//    std::vector<size_t> ans;
//    int n;
//    std::string s1, s2;
//    std::getline(std::cin, s1);
//    n = std::stoi(s1);
//    while (n) {
//        std::getline(std::cin, s1);
//        std::getline(std::cin, s2);
//        ans.push_back(levenshteinDistance(s1, s2));
//        n--;
//    }
//    for (size_t i = 0; i < ans.size(); ++i) {
//        if (i != 0) {
//            std::cout << ' ';
//        }
//        std::cout << ans[i];
//    }
//}