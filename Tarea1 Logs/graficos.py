import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in plt.style.available else 'default')
plt.rcParams.update({'font.size': 11, 'figure.titlesize': 13})

def load_and_aggregate(filename):
    """Carga el CSV y calcula promedios por configuracion (i, j)."""
    if not os.path.exists(filename):
        print(f"Error: No se encontro el archivo {filename}")
        return None
    df = pd.read_csv(filename)
    # Promediar las 10 ejecuciones de cada configuracion.
    # Solo columnas numericas (pandas 3.x ya no descarta 'Series' por si sola).
    num_cols = df.select_dtypes(include=[np.number]).columns.tolist()
    return df.groupby(['i', 'j', 'V', 'E'], as_index=False)[num_cols].mean()

def fit_constant(theoretical_vals, y):
    """Constante c por minimos cuadrados: min_c ||y - c*theoretical||^2."""
    return np.sum(y * theoretical_vals) / np.sum(theoretical_vals ** 2)

def plot_single(x, y_exp, y_theo, xlabel, ylabel, title, label, filename, y_limit=None):
    """Genera y guarda un grafico individual con su cota teorica ajustada."""
    plt.figure(figsize=(7, 5))
    plt.plot(x, y_exp, marker='o', linewidth=2, label=label, color='#1f77b4')
    plt.plot(x, y_theo, linestyle='--', linewidth=2, label='Cota Teorica ($c \\cdot f$)', color='#d62728')

    plt.title(title, pad=12)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    if y_limit:
        plt.ylim(0, y_limit)
    plt.legend()
    plt.tight_layout()
    plt.savefig(filename, dpi=300)
    plt.close()
    print(f"Guardado: {filename}")

def main():
    # Cotas teoricas del costo total
    bound_prim_bin = lambda v, e: e * np.log2(v)              # O(E log V)
    bound_prim_fib = lambda v, e: e + v * np.log2(v)          # O(E + V log V)

    # =========================================================================
    # 1. COSTO TOTAL (Seccion 6.3.1: Series A y B)
    # =========================================================================
    for series, fixed_var, xlabel_str in [('A', 'V=2^20', 'Cantidad de Aristas |E|'),
                                          ('B', 'E=2^24', 'Cantidad de Vertices |V|')]:
        df = load_and_aggregate(f'results_Serie{series}.csv')
        if df is None:
            continue

        x_vals = df['E'] if series == 'A' else df['V']
        # Misma escala para comparar ambas colas en la misma serie
        max_y = max(df['Time_Bin_MS'].max(), df['Time_Fib_MS'].max()) * 1.15

        theo_bin = np.array([bound_prim_bin(v, e) for v, e in zip(df['V'], df['E'])])
        theo_fib = np.array([bound_prim_fib(v, e) for v, e in zip(df['V'], df['E'])])

        c_bin = fit_constant(theo_bin, df['Time_Bin_MS'])
        plot_single(
            x_vals, df['Time_Bin_MS'], c_bin * theo_bin,
            xlabel_str, 'Tiempo Total de Ejecucion (ms)',
            f'Serie {series} ({fixed_var}) - Prim con Cola Binomial',
            'Binomial Heap Empirico', f'grafico_1_total_bin_serie_{series}.png', y_limit=max_y
        )

        c_fib = fit_constant(theo_fib, df['Time_Fib_MS'])
        plot_single(
            x_vals, df['Time_Fib_MS'], c_fib * theo_fib,
            xlabel_str, 'Tiempo Total de Ejecucion (ms)',
            f'Serie {series} ({fixed_var}) - Prim con Cola de Fibonacci',
            'Fibonacci Heap Empirico', f'grafico_2_total_fib_serie_{series}.png', y_limit=max_y
        )

    # =========================================================================
    # 2. COSTO AMORTIZADO DE DECREASEKEY (Seccion 6.3.2: Series C y D)
    # El eje X es la cantidad de llamadas a decreaseKey, como pide la guia.
    # Binomial: acumulado O(E log V) -> c*C*log2(V); Fibonacci: O(E) -> c*C.
    # =========================================================================
    for series, fixed_var in [('C', 'V=2^18'), ('D', 'E=2^22')]:
        df = load_and_aggregate(f'results_Serie{series}.csv')
        if df is None:
            continue

        calls = df['Calls_Bin']                 # mismas llamadas exitosas en ambas colas
        logv = np.log2(df['V'])
        theo_bin = calls * logv                 # O(C log V) acumulado
        theo_fib = calls                        # O(1) amortizado por llamada
        xlabel = 'Cantidad de llamadas a decreaseKey'

        # --- A) TIEMPO ACUMULADO ---
        y_t_bin = df['DecKeyTime_Bin_MS']
        y_t_fib = df['DecKeyTime_Fib_MS']
        max_y_t = max(y_t_bin.max(), y_t_fib.max()) * 1.15

        c_t_bin = fit_constant(theo_bin, y_t_bin)
        plot_single(
            calls, y_t_bin, c_t_bin * theo_bin,
            xlabel, 'Tiempo Acumulado DecreaseKey (ms)',
            f'Serie {series} ({fixed_var}) - Tiempo DecreaseKey (Binomial)',
            'Tiempo Binomial', f'grafico_3_time_bin_serie_{series}.png', y_limit=max_y_t
        )

        c_t_fib = fit_constant(theo_fib, y_t_fib)
        plot_single(
            calls, y_t_fib, c_t_fib * theo_fib,
            xlabel, 'Tiempo Acumulado DecreaseKey (ms)',
            f'Serie {series} ({fixed_var}) - Tiempo DecreaseKey (Fibonacci)',
            'Tiempo Fibonacci', f'grafico_4_time_fib_serie_{series}.png', y_limit=max_y_t
        )

        # --- B) CONTEO DE OPERACIONES ---
        y_o_bin = df['Ops_Bin']
        y_o_fib = df['Ops_Fib']
        max_y_o = max(y_o_bin.max(), y_o_fib.max()) * 1.15

        c_o_bin = fit_constant(theo_bin, y_o_bin)
        plot_single(
            calls, y_o_bin, c_o_bin * theo_bin,
            xlabel, 'Cantidad de Intercambios (Swaps)',
            f'Serie {series} ({fixed_var}) - Operaciones DecreaseKey (Binomial)',
            'Swaps Binomial', f'grafico_5_ops_bin_serie_{series}.png', y_limit=max_y_o
        )

        c_o_fib = fit_constant(theo_fib, y_o_fib)
        plot_single(
            calls, y_o_fib, c_o_fib * theo_fib,
            xlabel, 'Cantidad de Cortes (Cuts)',
            f'Serie {series} ({fixed_var}) - Operaciones DecreaseKey (Fibonacci)',
            'Cortes Fibonacci', f'grafico_6_ops_fib_serie_{series}.png', y_limit=max_y_o
        )

        # --- TABLA RESUMEN DE PROMEDIOS PARA EL INFORME ---
        print(f"\n--- TABLA RESUMEN PROMEDIOS SERIE {series} ---")
        summary_tbl = df[['i', 'j', 'V', 'E', 'Calls_Bin',
                          'Time_Bin_MS', 'Time_Fib_MS', 'Ops_Bin', 'Ops_Fib']]
        print(summary_tbl.to_string(index=False))

if __name__ == '__main__':
    main()