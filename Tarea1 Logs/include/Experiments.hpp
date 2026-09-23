#ifndef EXPERIMENTS_HPP
#define EXPERIMENTS_HPP

#include <string>
#include <vector>

/** Configuración (serie, i, j): grafo con V=2^i nodos y E=2^j aristas. */
struct Config {
    std::string series;
    int i;
    int j;
};

/**
 * Ejecuta una serie de configuraciones (i, j) sobre 'repetitions' grafos
 * aleatorios distintos, corriendo Prim con cola binomial y cola de Fibonacci.
 * Si measure_dec_key es true, cronometra cada decreaseKey por separado
 * (usado en las series C y D de costo amortizado).
 */
void run_series(const std::string& name, const std::vector<Config>& configs,
                int repetitions = 10, bool measure_dec_key = false);

#endif