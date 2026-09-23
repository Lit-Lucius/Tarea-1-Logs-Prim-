#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

struct Edge {
    int to;
    double weight;
};

struct Graph {
    int num_v;
    long long num_e;
    std::vector<std::vector<Edge>> adj;

    Graph(int v);
    void add_edge(int u, int v, double w);
};

/**
 * Genera un grafo conexo aleatorio utilizando el método sugerido en la guía.
 */
Graph generate_random_connected_graph(int v, long long e, unsigned int seed = 42);

#endif