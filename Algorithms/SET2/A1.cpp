#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

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

std::vector<size_t> visited;

void dfs(size_t vert, std::vector<std::list<size_t>> &graph) {
    visited[vert] = 1;
    for (auto &to: graph[vert]) {
        if (!visited[to]) {
            dfs(to, graph);
        }
    }
}

std::vector<Edge> alg1(size_t n, size_t m, std::vector<Edge> graph, std::vector<std::list<size_t>> adj_list) {
    std::sort(graph.begin(), graph.end());
    auto mst = graph;
    for (int i = m - 1; i >= 0; --i) {
        std::vector<Edge> mst_copy;
        mst_copy.reserve(mst.size());
        for (int j = 0; j < mst.size(); j++) {
            if (j != i)
                mst_copy.push_back(mst[j]);
        }
        visited = std::vector<size_t>(n, 0);
        adj_list[graph[i].from].erase(
                find(adj_list[graph[i].from].begin(), adj_list[graph[i].from].end(), graph[i].to));
        adj_list[graph[i].to].erase(find(adj_list[graph[i].to].begin(), adj_list[graph[i].to].end(), graph[i].from));
        dfs(0, adj_list);
        if (std::count(visited.begin(), visited.end(), 1) == visited.size()) {
            mst = mst_copy;
        } else {
            adj_list[graph[i].from].push_back(graph[i].to);
            adj_list[graph[i].to].push_back(graph[i].from);
        }
    }
    return mst;
}

std::vector<size_t> parent;

size_t find(size_t u) {
    if (parent[u] == u) {
        return u;
    }
    return find(parent[u]);
}

bool unite(size_t u, size_t v) {
    auto pu = find(u), pv = find(v);
    if (pu != pv) {
        parent[pu] = pv;
        return true;
    }
    return false;
}

std::vector<Edge> alg2(size_t n, std::vector<Edge> &graph) {
    std::vector<Edge> mst;
    parent.resize(n);
    for (size_t i = 0; i < n; ++i) {
        parent[i] = i;
    }
    for (auto &edge: graph) {
        if (unite(edge.from, edge.to)) {
            mst.push_back(edge);
        }
    }
    return mst;
}

bool dfsCycle(size_t vert, std::vector<std::list<Edge>> &graph, Edge &eMax) {
    visited[vert] = 1;
    for (auto &edge: graph[vert]) {
        if (!visited[edge.to]) {
            parent[edge.to] = vert;
            if (dfsCycle(edge.to, graph, eMax)) {
                if (edge.weight > eMax.weight) {
                    eMax = edge;
                }
                return true;
            }
        } else if (edge.to != parent[vert]) {
            if (edge.weight > eMax.weight) {
                eMax = edge;
            }
            return true;
        }
    }
    return false;
}

bool hasCycle(std::vector<std::list<Edge>> &graph, Edge &eMax) {
    size_t n = graph.size();
    visited = std::vector<size_t>(n, 0);
    parent = std::vector<size_t>(n, -1);
    for (size_t i = 0; i < n; ++i) {
        if (!visited[i] && dfsCycle(i, graph, eMax)) {
            return true;
        }
    }
    return false;
}

std::vector<Edge> alg3(size_t n, size_t m, std::vector<Edge> graph) {
    std::vector<Edge> mst;
    std::vector<std::list<Edge>> adj_list = std::vector<std::list<Edge>>(n, std::list<Edge>());
    for (size_t i = 0; i < m; ++i) {
        auto maxEdge = Edge(0, 0, 0);
        adj_list[graph[i].from].push_back(graph[i]);
        adj_list[graph[i].to].emplace_back(graph[i].to, graph[i].from, graph[i].weight);
        mst.push_back(graph[i]);
        if (hasCycle(adj_list, maxEdge)) {
            adj_list[maxEdge.from].erase(
                    find(adj_list[maxEdge.from].begin(), adj_list[maxEdge.from].end(), maxEdge));
            adj_list[maxEdge.to].erase(
                    find(adj_list[maxEdge.to].begin(), adj_list[maxEdge.to].end(), Edge(maxEdge.to, maxEdge.from,
                                                                                          maxEdge.weight)));
            auto it = std::find(mst.begin(), mst.end(), maxEdge);
            if (it != mst.end()) {
                mst.erase(it);
            } else {
                it = std::find(mst.begin(), mst.end(), Edge(maxEdge.to, maxEdge.from, maxEdge.weight));
                if (it != mst.end()) {
                    mst.erase(it);
                }
            }

        }
    }
    return mst;
}

void showMST(std::vector<Edge> mst) {
    size_t sum = 0;
    for (auto &edge: mst) {
        sum += edge.weight;
    }
    std::cout << sum << std::endl;
    for (auto &edge: mst) {
        std::cout << edge.from + 1 << " " << edge.to + 1 << std::endl;
    }
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    std::vector<Edge> graph(m);
    std::vector<std::list<size_t>> adj_list = std::vector<std::list<size_t>>(n, std::list<size_t>());
    for (size_t i = 0; i < m; ++i) {
        std::cin >> graph[i].from >> graph[i].to >> graph[i].weight;
        graph[i].from--;
        graph[i].to--;
        adj_list[graph[i].from].push_back(graph[i].to);
        adj_list[graph[i].to].push_back(graph[i].from);

    }
    showMST(alg1(n, m, graph, adj_list));
    showMST(alg2(n, graph));
    showMST(alg3(n, m, graph));
    return 0;
}