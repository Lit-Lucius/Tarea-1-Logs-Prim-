# CC4102 - Tarea 1: Algoritmo de Prim y Análisis Amortizado

Implementación en C++ del algoritmo de Prim para el Árbol Cobertor Mínimo (MST) usando dos
colas de prioridad implementadas desde cero: **colas binomiales** (decreaseKey `O(log V)` en
el peor caso) y **colas de Fibonacci** (decreaseKey `O(1)` amortizado). Incluye un generador
de grafos conexos aleatorios, una batería de experimentos (series A, B, C y D) y un script
para generar los gráficos del informe.

## Requisitos

- **Linux o WSL**: el `MakeFile` usa comandos POSIX (`mkdir -p`, `rm -rf`).
- `g++` con soporte C++17 y `make`.
- Python 3 con `numpy`, `pandas` y `matplotlib` (solo para generar los gráficos).

## Compilación

```bash
make
```

Compila con `-O3 -std=c++17 -Iinclude` y genera el binario `main`. Alternativa sin `make`:

```bash
g++ -O3 -std=c++17 -Iinclude src/*.cpp -o main
```

## Ejecución

```bash
./main                 # ejecuta las 4 series completas con 10 repeticiones
./main SerieA          # ejecuta solo una serie
./main SerieA SerieC   # ejecuta varias series (argumentos = nombres de serie)
```

- **Serie A** (`i=20`, `j=20..24`) y **Serie B** (`j=24`, `i=18..22`): miden el costo total.
  Generan `results_SerieA.csv` y `results_SerieB.csv`.
- **Serie C** (`i=18`, `j=18..22`) y **Serie D** (`j=22`, `i=14..18`): miden el costo
  amortizado de decreaseKey (tiempo acumulado y conteo de operaciones por llamada).
  Generan `results_SerieC.csv` y `results_SerieD.csv`.

Cada configuración `(i, j)` usa un grafo con `V = 2^i` nodos y `E = 2^j` aristas, se repite
10 veces con un grafo aleatorio distinto (semilla determinística `1000 + rep*17`) para
reproducibilidad, y se verifica con un `assert` que ambas implementaciones obtienen un MST
del mismo peso total.

### Mediciones

- El tiempo total mide desde la construcción inicial de la cola hasta el final del algoritmo
  (la generación del grafo no se incluye).
- El cronometraje individual de cada `decreaseKey` se activa solo en las series C y D
  (flag `measure_dec_key`). Así el overhead del reloj no infla el tiempo total de las series
  A y B, que es el dato que se compara contra las cotas de costo total.

## Resultados

Cada CSV contiene, por repetición y configuración: peso del MST, tiempo total, tiempo
acumulado de decreaseKey, cantidad de operaciones estructurales (swaps para binomial, cortes
para Fibonacci) y cantidad de llamadas exitosas a decreaseKey, para ambas implementaciones.

## Gráficos

```bash
python3 graficos.py
```

Genera los 12 gráficos en PNG (costo total de las series A y B; tiempo acumulado y conteo de
operaciones de las series C y D), cada uno con su cota teórica ajustada por una constante
obtenida por mínimos cuadrados. Dentro de una misma serie y forma de medición, ambos gráficos
comparten la escala del eje Y para compararlos directamente. Además imprime tablas de
promedios por configuración útiles para el informe.

## Estructura del proyecto

```
include/     Cabeceras con definiciones y firma (propósito) de cada estructura y función
src/         Implementaciones: colas binomial y de Fibonacci, grafo, Prim, experimentos, main
graficos.py  Generación de los 12 gráficos a partir de los CSVs
MakeFile     Compilación del programa
```

## Implementación

- **Cola binomial**: `decreaseKey` por intercambio de contenido (`key` y `vertex` a la vez);
  los punteros `node_ptr` de cada vértice se actualizan junto con cada swap.
- **Cola de Fibonacci**: `decreaseKey` por corte directo del nodo más cortes en cascada
  (marcando nodos que pierden un hijo), logrando O(1) amortizado.
- **Grafo**: lista de adyacencia con aristas no dirigidas y pesos aleatorios en el rango
  `(0.0001, 1.0]`; conectividad garantizada agregando un árbol cobertor aleatorio y luego
  las aristas restantes descartando reflexivas y repetidas.