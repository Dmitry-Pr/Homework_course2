//#include <iostream>
//#include <map>
//#include <vector>
//
//int main() {
//    std::string s;
//    std::getline(std::cin, s);
//
//    std::map<std::string, int> dictionary;
//    for (int i = 0; i <= 127; ++i) {
//        std::string ch(1, char(i));
//        dictionary[ch] = i;
//    }
//
//    std::vector<int> result;
//    std::string current_string;
//    current_string += s[0];
//
//    int code = 128;
//    for (int i = 1; i < s.size(); ++i) {
//        current_string += s[i];
//        if (dictionary.find(current_string) == dictionary.end()) {
//            dictionary[current_string] = code;
//            current_string.erase(current_string.size() - 1);
//            result.push_back(dictionary[current_string]);
//            current_string = s[i];
//            ++code;
//        }
//    }
//    result.push_back(dictionary[current_string]);
//
//    std::cout << result.size() << std::endl;
//    for (auto &code : result) {
//        std::cout << code << " ";
//    }
//    std::cout << std::endl;
//
//    return 0;
//}