//#include <iostream>
//#include <vector>
//#include <climits>
//#include <cstdint>
//
//struct Edge {
//    uint64_t from;
//    uint64_t to;
//    uint64_t weight;
//
//    bool operator==(const Edge &edge) const {
//        return from == edge.from && to == edge.to && weight == edge.weight;
//    }
//};
//
//
//std::istream &operator>>(std::istream &in, Edge &edge) {
//    in >> edge.from >> edge.to >> edge.weight;
//    return in;
//}
//
//size_t n, m;
//std::vector<Edge> graph;
//
//void floydWarshall() {
//    std::vector<std::vector<uint64_t>> dist(n, std::vector<uint64_t>(n, INT64_MAX));
//    for (size_t i = 0; i < n; ++i) {
//        dist[i][i] = 0;
//    }
//    for (auto &edge: graph) {
//        dist[edge.from][edge.to] = edge.weight;
//    }
//    for (size_t k = 0; k < n; ++k) {
//        for (size_t i = 0; i < n; ++i) {
//            for (size_t j = 0; j < n; ++j) {
//                if (dist[i][k] + dist[k][j] < dist[i][j]) {
//                    dist[i][j] = dist[i][k] + dist[k][j];
//                }
//            }
//        }
//    }
//    for (size_t i = 0; i < n; ++i) {
//        for (size_t j = 0; j < n; ++j) {
//            if (i == j) {
//                continue;
//            }
//            if (dist[i][j] == INT64_MAX) {
//                std::cout << i << ' ' << j << ' ' << -1 << '\n';
//            } else {
//                std::cout << i << ' ' << j << ' ' << dist[i][j] << '\n';
//            }
//        }
//    }
//
//}
//
//int main() {
//
//    std::cin >> n >> m;
//
//    graph.resize(m);
//
//    for (size_t i = 0; i < m; ++i) {
//        std::cin >> graph[i];
//    }
//    floydWarshall();
//}