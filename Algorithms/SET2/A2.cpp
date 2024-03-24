#include <vector>
#include <list>
#include <iostream>

struct Edge {
    size_t from;
    size_t to;
    size_t weight;

    Edge() = default;

    Edge(size_t from, size_t to, size_t weight) : from(from), to(to), weight(weight) {}

    bool operator==(const Edge &edge) const {
        return from == edge.from && to == edge.to && weight == edge.weight;
    }

    bool operator<(const Edge &edge) const {
        return weight < edge.weight;
    }
};

std::vector<std::list<Edge>> RestoreGraph(const std::vector<std::vector<int>> &dist) {
    int n = dist.size();
    std::vector<std::list<Edge>> graph(n);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            graph[i].push_back(Edge(i, j, dist[i][j]));
            graph[j].push_back(Edge(j, i, dist[i][j]));
        }
    }

    return graph;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> dist(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> dist[i][j];
        }
    }

    auto graph = RestoreGraph(dist);
    for (int i = 0; i < n; ++i) {
        std::cout << i + 1 << ": ";
        for (auto edge: graph[i]) {
            std::cout << "(" << edge.to + 1 << ", " << edge.weight << ") ";
        }
        std::cout << std::endl;
    }

    return 0;
}

//3
//0 3 5
//3 0 1
//5 1 0

