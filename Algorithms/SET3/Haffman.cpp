//#include <iostream>
//#include <queue>
//#include <vector>
//#include <map>
//
//struct Node {
//    char ch;
//    int freq;
//    Node *left;
//    Node *right;
//};
//
//Node *getNode(char ch, int freq, Node *left, Node *right) {
//    Node *node = new Node();
//    node->ch = ch;
//    node->freq = freq;
//    node->left = left;
//    node->right = right;
//    return node;
//}
//
//struct comp {
//    bool operator()(Node *l, Node *r) {
//        return l->freq > r->freq;
//    }
//};
//
//void encode(Node *root, std::string str, std::map<char, std::string> &huffmanCode) {
//    if (root == nullptr)
//        return;
//
//    if (!root->left && !root->right) {
//        if (str.empty())
//            huffmanCode[root->ch] = "0";
//        else
//            huffmanCode[root->ch] = str;
//    }
//
//    encode(root->left, str + "0", huffmanCode);
//    encode(root->right, str + "1", huffmanCode);
//}
//
//void deleteTree(Node *root) {
//    if (root == nullptr) {
//        return;
//    }
//    deleteTree(root->left);
//    deleteTree(root->right);
//    delete root;
//}
//
//void buildHuffmanTree(std::string text) {
//    std::map<char, int> freq;
//    for (char ch: text) {
//        freq[ch]++;
//    }
//
//    std::priority_queue<Node *, std::vector<Node *>, comp> pq;
//
//    for (auto pair: freq) {
//        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
//    }
//
//    while (pq.size() != 1) {
//        Node *left = pq.top();
//        pq.pop();
//        Node *right = pq.top();
//        pq.pop();
//
//        int sum = left->freq + right->freq;
//        pq.push(getNode('\0', sum, left, right));
//    }
//
//    Node *root = pq.top();
//
//    std::map<char, std::string> huffmanCode;
//    encode(root, "", huffmanCode);
//
//    std::string str = "";
//    for (char ch: text) {
//        str += huffmanCode[ch];
//    }
//
//    std::cout << freq.size() << " " << str.length() << std::endl;
//    for (auto pair: huffmanCode) {
//        std::cout << pair.first << ": " << pair.second << std::endl;
//    }
//
//
//    std::cout << str << std::endl;
//    deleteTree(root);
//}
//
//
//int main() {
//    std::string text;
//    std::cin >> text;
//    buildHuffmanTree(text);
//    return 0;
//}