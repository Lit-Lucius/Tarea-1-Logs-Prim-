#include "Experiments.hpp"
#include "Prim.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cassert>

void run_series(const std::string& name, const std::vector<Config>& configs,
                int repetitions, bool measure_dec_key) {
    std::cout << "\n======================================================\n";
    std::cout << " Ejecutando Experimentos - " << name << "\n";
    std::cout << "======================================================\n";

    std::ofstream csv("results_" + name + ".csv");
    csv << "Series,i,j,V,E,Rep,Weight_Bin,Time_Bin_MS,DecKeyTime_Bin_MS,Ops_Bin,Calls_Bin,"
        << "Weight_Fib,Time_Fib_MS,DecKeyTime_Fib_MS,Ops_Fib,Calls_Fib\n";

    for (const auto& cfg : configs) {
        int v = 1 << cfg.i;
        long long e = 1LL << cfg.j;

        std::cout << "Config: i=" << cfg.i << ", j=" << cfg.j
                  << " (V=" << v << ", E=" << e << ") | " << repetitions << " repeticiones...\n";

        double avg_time_bin = 0, avg_time_fib = 0;
        double avg_dk_bin = 0, avg_dk_fib = 0;
        long long avg_ops_bin = 0, avg_ops_fib = 0;
        long long avg_calls_bin = 0, avg_calls_fib = 0;

        for (int rep = 0; rep < repetitions; ++rep) {
            Graph g = generate_random_connected_graph(v, e, 1000 + rep * 17);

            PrimResult res_bin = prim_binomial(g, 0, measure_dec_key);
            PrimResult res_fib = prim_fibonacci(g, 0, measure_dec_key);

            assert(std::abs(res_bin.total_weight - res_fib.total_weight) < 1e-6);

            avg_time_bin += res_bin.total_time_ms;
            avg_time_fib += res_fib.total_time_ms;
            avg_dk_bin += res_bin.dec_key_time_ms;
            avg_dk_fib += res_fib.dec_key_time_ms;
            avg_ops_bin += res_bin.dec_key_ops;
            avg_ops_fib += res_fib.dec_key_ops;
            avg_calls_bin += res_bin.dec_key_calls;
            avg_calls_fib += res_fib.dec_key_calls;

            csv << cfg.series << "," << cfg.i << "," << cfg.j << "," << v << "," << e << ","
                << (rep + 1) << "," << std::fixed << std::setprecision(6)
                << res_bin.total_weight << "," << res_bin.total_time_ms << ","
                << res_bin.dec_key_time_ms << "," << res_bin.dec_key_ops << ","
                << res_bin.dec_key_calls << ","
                << res_fib.total_weight << "," << res_fib.total_time_ms << ","
                << res_fib.dec_key_time_ms << "," << res_fib.dec_key_ops << ","
                << res_fib.dec_key_calls << "\n";
        }

        std::cout << "  -> PROMEDIOS | Binomial: " << (avg_time_bin / repetitions) << " ms, "
                  << (avg_ops_bin / repetitions) << " swaps, "
                  << (avg_calls_bin / repetitions) << " dk | Fibonacci: "
                  << (avg_time_fib / repetitions) << " ms, "
                  << (avg_ops_fib / repetitions) << " cortes, "
                  << (avg_calls_fib / repetitions) << " dk\n";
    }

    csv.close();
    std::cout << "Resultados guardados en 'results_" << name << ".csv'\n";
}