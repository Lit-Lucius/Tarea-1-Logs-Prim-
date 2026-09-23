#include "Graph.hpp"
#include <random>
#include <unordered_set>
#include <algorithm>
#include <cstdint>

Graph::Graph(int v) : num_v(v), num_e(0), adj(v) {}

void Graph::add_edge(int u, int v, double w) {
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
    num_e++;
}

Graph generate_random_connected_graph(int v, long long e, unsigned int seed) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist_w(0.0001, 1.0);

    Graph g(v);
    std::unordered_set<uint64_t> existing_edges;
    existing_edges.reserve(e);

    auto make_pair_key = [](int u, int w) -> uint64_t {
        int a = std::min(u, w);
        int b = std::max(u, w);
        return ((uint64_t)a << 32) | (uint32_t)b;
    };

    for (int i = 1; i < v; ++i) {
        std::uniform_int_distribution<int> dist_parent(0, i - 1);
        int parent = dist_parent(rng);
        double w = dist_w(rng);
        g.add_edge(i, parent, w);
        existing_edges.insert(make_pair_key(i, parent));
    }

    long long remaining_edges = e - (v - 1);
    std::uniform_int_distribution<int> dist_v(0, v - 1);

    while (remaining_edges > 0) {
        int u = dist_v(rng);
        int w_node = dist_v(rng);
        if (u == w_node) continue;

        uint64_t key = make_pair_key(u, w_node);
        if (existing_edges.find(key) == existing_edges.end()) {
            existing_edges.insert(key);
            double w = dist_w(rng);
            g.add_edge(u, w_node, w);
            remaining_edges--;
        }
    }

    return g;
}