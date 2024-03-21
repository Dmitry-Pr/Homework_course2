//#include <iostream>
//#include <vector>
//#include <stack>
//#include <list>
//
//std::vector<std::list<size_t>> adjList;
//std::vector<std::list<size_t>> reversedAdjList;
//std::vector<size_t> visited;
//std::stack<size_t> readyStack;
//
//void DFS(size_t vert) {
//    visited[vert] = 1;
//    for (auto &neighbour: adjList[vert]) {
//        if (!visited[neighbour]) {
//            DFS(neighbour);
//        }
//    }
//    readyStack.push(vert);
//}
//
//void stDFS(size_t vert, size_t comp) {
//    visited[vert] = comp;
//    for (auto &neighbour: reversedAdjList[vert]) {
//        if (!visited[neighbour]) {
//            stDFS(neighbour, comp);
//        }
//    }
//}
//
//void input() {
//    size_t n, m;
//    std::cin >> n >> m;
//    adjList.resize(n);
//    reversedAdjList.resize(n);
//    visited.resize(n);
//    for (size_t i = 0; i < m; ++i) {
//        size_t from, to;
//        std::cin >> from >> to;
//        adjList[from - 1].push_back(to - 1);
//        reversedAdjList[to - 1].push_back(from - 1);
//    }
//}
//
//int main() {
//    input();
//    for (size_t i = 0; i < adjList.size(); ++i) {
//        if (!visited[i]) {
//            DFS(i);
//        }
//    }
//    visited = std::vector<size_t>(visited.size(), 0);
//    size_t comp = 1;
//    while (!readyStack.empty()) {
//        size_t vert = readyStack.top();
//        readyStack.pop();
//        if (!visited[vert]) {
//            stDFS(vert, comp);
//            ++comp;
//        }
//    }
//    std::cout << comp - 1 << std::endl;
//    for (auto &i: visited) {
//        std::cout << i << " ";
//    }
//    std::cout << '\n';
//    return 0;
//}
