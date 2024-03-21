//#include <iostream>
//#include <vector>
//#include <queue>
//
//using namespace std;
//
//struct Edge {
//    size_t from;
//    size_t to;
//    long long capacity;
//    size_t reverseEdge;
//
//    bool operator==(const Edge &edge) const {
//        return from == edge.from && to == edge.to && capacity == edge.capacity;
//    }
//};
//
//
//std::istream &operator>>(std::istream &in, Edge &edge) {
//    in >> edge.from >> edge.to >> edge.capacity;
//    edge.from--;
//    edge.to--;
//    return in;
//}
//
//int n;
//int m;
//vector<vector<Edge>> graph;
//vector<int> levels;
//
//
//bool bfs(size_t start, size_t end) {
//    for (auto &i: levels) {
//        i = -1;
//    }
//    levels[start] = 0;
//    queue<size_t> q;
//    q.push(start);
//    while (!q.empty()) {
//        int cur = q.front();
//        q.pop();
//        for (const Edge &edge: graph[cur]) {
//            if (levels[edge.to] < 0 && edge.capacity > 0) {
//                levels[edge.to] = levels[edge.from] + 1;
//                q.push(edge.to);
//            }
//        }
//    }
//    return levels[end] >= 0;
//}
//
//long long dfs(size_t cur, size_t end, long long curFlow) {
//    if (cur == end) {
//        return curFlow;
//    }
//    for (Edge &edge: graph[cur]) {
//        if (levels[edge.to] == levels[cur] + 1 && edge.capacity > 0) {
//            long long flow = dfs(edge.to, end, curFlow < edge.capacity ? curFlow : edge.capacity);
//            if (flow > 0) {
//                edge.capacity -= flow;
//                graph[edge.to][edge.reverseEdge].capacity += flow;
//                return flow;
//            }
//        }
//    }
//    return 0;
//}
//
//long long dinic(int start, int end) {
//    long long resFlow = 0;
//    while (bfs(start, end)) {
//        long long flow;
//        while ((flow = dfs(start, end, 1e10)) > 0) {
//            resFlow += flow;
//        }
//    }
//    return resFlow;
//}
//
//int main() {
//
//    std::cin >> n >> m;
//
//    graph.resize(m);
//    levels.resize(n);
//
//    for (size_t i = 0; i < m; ++i) {
//        Edge edge;
//        std::cin >> edge;
//        graph[edge.from].push_back({edge.from, edge.to, edge.capacity, graph[edge.to].size()});
//        graph[edge.to].push_back({edge.to, edge.from, 0, graph[edge.from].size() - 1});
//    }
//    cout << dinic(0, n - 1) << endl;
//
//    return 0;
//}
