#ifndef PRIM_HPP
#define PRIM_HPP

#include "Graph.hpp"

/**
 * Resultado de la ejecución de Prim.
 * total_time_ms incluye la construcción inicial de la cola y todo el algoritmo.
 * dec_key_time_ms solo se mide cuando measure_dec_key es true (series C y D),
 * para no inflar el tiempo total de las series A y B con el overhead del reloj.
 */
struct PrimResult {
    double total_weight;        // Peso total del MST encontrado.
    double total_time_ms;       // Tiempo total del algoritmo (incluye construcción).
    double dec_key_time_ms;     // Tiempo acumulado de todas las decreaseKey.
    long long dec_key_ops;      // Operaciones estructurales (swaps / cortes).
    long long dec_key_calls;    // Cantidad de decreaseKey exitosos.
};

/**
 * Algoritmo de Prim usando cola binomial (decreaseKey O(log V) peor caso).
 * Si measure_dec_key es true, cronometra cada decreaseKey por separado.
 */
PrimResult prim_binomial(const Graph& g, int root = 0, bool measure_dec_key = true);

/**
 * Algoritmo de Prim usando cola de Fibonacci (decreaseKey O(1) amortizado).
 * Si measure_dec_key es true, cronometra cada decreaseKey por separado.
 */
PrimResult prim_fibonacci(const Graph& g, int root = 0, bool measure_dec_key = true);

#endif