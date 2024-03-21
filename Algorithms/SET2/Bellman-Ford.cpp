//#include <iostream>
//#include <vector>
//#include <climits>
//
//struct Edge {
//    long long from;
//    long long to;
//    long long weight;
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
//void bellmanFord(size_t start) {
//    std::vector<long long> dist(n, INT_MAX);
//    dist[start] = 0;
//    for (size_t i = 0; i < n - 1; ++i) {
//        for (auto &edge: graph) {
//            if (dist[edge.from] < INT_MAX && dist[edge.from] + edge.weight < dist[edge.to]) {
//                dist[edge.to] = dist[edge.from] + edge.weight;
//            }
//        }
//    }
//    for (size_t i = 0; i < n; i++) {
//        for (auto &edge: graph) {
//            if (dist[edge.from] < INT_MAX && dist[edge.from] + edge.weight < dist[edge.to]) {
//                dist[edge.to] = INT_MIN;
//
//            }
//        }
//    }
//    for (size_t i = 1; i < n; ++i) {
//        if (dist[i] == INT_MIN) {
//            std::cout << "-inf\n";
//        } else {
//            std::cout << dist[i] << "\n";
//        }
//    }
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
//    bellmanFord(0);
//}