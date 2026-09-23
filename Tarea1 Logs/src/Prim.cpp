#include "Prim.hpp"
#include "BinomialHeap.hpp"
#include "FibonacciHeap.hpp"
#include <chrono>
#include <limits>

PrimResult prim_binomial(const Graph& g, int root, bool measure_dec_key) {
    int v = g.num_v;
    std::vector<double> costos(v, std::numeric_limits<double>::infinity());
    std::vector<int> parent(v, -1);
    std::vector<bool> in_mst(v, false);

    costos[root] = 0.0;
    BinomialHeap q(v);

    double mst_weight = 0.0;
    double dec_key_time = 0.0;

    // El tiempo total mide desde la construccion de la cola hasta el final.
    auto t_start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < v; ++i) {
        q.insert(i, costos[i]);
    }

    while (!q.empty()) {
        auto [u, c] = q.extract_min();
        if (u == -1) break;

        in_mst[u] = true;
        mst_weight += c;

        for (const auto& edge : g.adj[u]) {
            int neighbor = edge.to;
            double w = edge.weight;

            if (!in_mst[neighbor] && w < costos[neighbor]) {
                costos[neighbor] = w;
                parent[neighbor] = u;

                if (measure_dec_key) {
                    auto dk_start = std::chrono::high_resolution_clock::now();
                    q.decrease_key(neighbor, w);
                    auto dk_end = std::chrono::high_resolution_clock::now();
                    dec_key_time += std::chrono::duration<double, std::milli>(dk_end - dk_start).count();
                } else {
                    q.decrease_key(neighbor, w);
                }
            }
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    double total_time = std::chrono::duration<double, std::milli>(t_end - t_start).count();

    return {mst_weight, total_time, dec_key_time, q.op_count, q.call_count};
}

PrimResult prim_fibonacci(const Graph& g, int root, bool measure_dec_key) {
    int v = g.num_v;
    std::vector<double> costos(v, std::numeric_limits<double>::infinity());
    std::vector<int> parent(v, -1);
    std::vector<bool> in_mst(v, false);

    costos[root] = 0.0;
    FibonacciHeap q(v);

    double mst_weight = 0.0;
    double dec_key_time = 0.0;

    auto t_start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < v; ++i) {
        q.insert(i, costos[i]);
    }

    while (!q.empty()) {
        auto [u, c] = q.extract_min();
        if (u == -1) break;

        in_mst[u] = true;
        mst_weight += c;

        for (const auto& edge : g.adj[u]) {
            int neighbor = edge.to;
            double w = edge.weight;

            if (!in_mst[neighbor] && w < costos[neighbor]) {
                costos[neighbor] = w;
                parent[neighbor] = u;

                if (measure_dec_key) {
                    auto dk_start = std::chrono::high_resolution_clock::now();
                    q.decrease_key(neighbor, w);
                    auto dk_end = std::chrono::high_resolution_clock::now();
                    dec_key_time += std::chrono::duration<double, std::milli>(dk_end - dk_start).count();
                } else {
                    q.decrease_key(neighbor, w);
                }
            }
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    double total_time = std::chrono::duration<double, std::milli>(t_end - t_start).count();

    return {mst_weight, total_time, dec_key_time, q.op_count, q.call_count};
}