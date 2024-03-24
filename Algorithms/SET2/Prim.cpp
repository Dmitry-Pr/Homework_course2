//#include <iostream>
//#include <vector>
//#include <climits>
//
//struct Edge {
//    size_t from;
//    size_t to;
//    size_t weight;
//
//    bool operator==(const Edge &edge) const {
//        return from == edge.from && to == edge.to && weight == edge.weight;
//    }
//};
//
//
//std::istream &operator>>(std::istream &in, Edge &edge) {
//    in >> edge.from >> edge.to >> edge.weight;
//    edge.from--;
//    edge.to--;
//    return in;
//}
//
//size_t n, m;
//std::vector<std::vector<size_t>> graph;
//std::vector<Edge> mst;
//std::vector<bool> visited;
//
//size_t prim() {
//    size_t visitedCount = 0;
//    size_t sum = 0;
//    visited[0] = true;
//    visitedCount++;
//    while (visitedCount < n) {
//        size_t minWeight = INT_MAX;
//        Edge minEdge;
//        for (size_t i = 0; i < n; ++i) {
//            if (visited[i]) {
//                for (size_t j = 0; j < n; ++j) {
//                    if (!visited[j] && graph[i][j] != 0 && graph[i][j] < minWeight) {
//                        minWeight = graph[i][j];
//                        minEdge = {i, j, minWeight};
//                    }
//                }
//            }
//        }
//        visited[minEdge.to] = true;
//        visitedCount++;
//        sum += minEdge.weight;
//        mst.push_back(minEdge);
//    }
//    return sum;
//}
//
//size_t prim2(Edge &edge, size_t &minSum) {
//    size_t visitedCount = 0;
//    size_t sum = 0;
//    visited[0] = true;
//    visitedCount++;
//    while (visitedCount < n) {
//        size_t minWeight = INT_MAX;
//        Edge minEdge;
//        for (size_t i = 0; i < n; ++i) {
//            if (visited[i]) {
//                for (size_t j = 0; j < n; ++j) {
//                    if (i == edge.from && j == edge.to) continue;
//                    if (!visited[j] && graph[i][j] != 0 && graph[i][j] < minWeight) {
//                        minWeight = graph[i][j];
//                        minEdge = {i, j, minWeight};
//                    }
//                }
//            }
//        }
//        visited[minEdge.to] = true;
//        visitedCount++;
//        sum += minEdge.weight;
//        mst.push_back(minEdge);
//    }
//    if (sum < minSum) {
//        return INT_MAX;
//    }
//        return sum;
//}
//
//int main() {
//
//    std::cin >> n >> m;
//
//    graph = std::vector<std::vector<size_t>>(n, std::vector<size_t>(n, 0));
//    visited.resize(n);
//
//    for (size_t i = 0; i < m; ++i) {
//        size_t k, j, w;
//        std::cin >> k >> j >> w;
//        graph[--k][--j] = w;
//        graph[j][k] = w;
//
//    }
//    auto sum1 = prim();
//    size_t minSum = INT_MAX;
//    std::vector<Edge> mst1(mst);
//    for (auto &edge: mst1) {
//        visited = std::vector<bool>(n, false);
//        mst.clear();
//        auto sum2 = prim2(edge, minSum);
//        minSum = sum2 < minSum ? sum2 : minSum;
//    }
//    std::cout << sum1 << ' ' << minSum;
//    return 0;
//}
//// reload