#include <iostream>
#include <vector>
#include <map>

int main() {
   int k;
   std::cin >> k;
   std::vector<int> codes(k);
   for (int i = 0; i < k; i++) {
       std::cin >> codes[i];
   }

   std::map<int, std::string> dictionary;
   for (int i = 0; i <= 127; i++) {
       dictionary[i] = i;
   }

   std::string result = dictionary[codes[0]];
   std::string s = dictionary[codes[0]];
   std::string entry;
   for (int i = 1; i < k; i++) {
       if (dictionary.count(codes[i])) {
           entry = dictionary[codes[i]];
       } else if (codes[i] == dictionary.size()) {
           entry = s + s[0];
       }
       result += entry;
       dictionary[dictionary.size()] = s + entry[0];
       s = entry;
   }

   std::cout << result;

   return 0;
}
