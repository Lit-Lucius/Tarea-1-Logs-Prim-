#include "Experiments.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::cout << "CC4102 - Tarea 1: Algoritmo de Prim (Binomial vs Fibonacci)\n";

    std::vector<Config> series_A = {
        {"SerieA", 20, 20}, {"SerieA", 20, 21}, {"SerieA", 20, 22},
        {"SerieA", 20, 23}, {"SerieA", 20, 24}
    };

    std::vector<Config> series_B = {
        {"SerieB", 18, 24}, {"SerieB", 19, 24}, {"SerieB", 20, 24},
        {"SerieB", 21, 24}, {"SerieB", 22, 24}
    };

    std::vector<Config> series_C = {
        {"SerieC", 18, 18}, {"SerieC", 18, 19}, {"SerieC", 18, 20},
        {"SerieC", 18, 21}, {"SerieC", 18, 22}
    };

    std::vector<Config> series_D = {
        {"SerieD", 14, 22}, {"SerieD", 15, 22}, {"SerieD", 16, 22},
        {"SerieD", 17, 22}, {"SerieD", 18, 22}
    };

    std::vector<std::string> selected;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) selected.push_back(argv[i]);
    } else {
        selected = {"SerieA", "SerieB", "SerieC", "SerieD"};
    }

    auto want = [&](const std::string& s) {
        return std::find(selected.begin(), selected.end(), s) != selected.end();
    };

    if (want("SerieA")) run_series("SerieA", series_A, 10, false);
    if (want("SerieB")) run_series("SerieB", series_B, 10, false);
    if (want("SerieC")) run_series("SerieC", series_C, 10, true);
    if (want("SerieD")) run_series("SerieD", series_D, 10, true);

    std::cout << "\n¡Experimentos completados exitosamente!\n";
    return 0;
}