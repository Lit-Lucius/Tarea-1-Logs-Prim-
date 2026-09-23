#ifndef FIBONACCI_HEAP_HPP
#define FIBONACCI_HEAP_HPP

#include <vector>
#include <utility>

/**
 * Nodo de una cola de Fibonacci.
 * Guarda el vértice representado, su clave, su grado y el flag de "nodo marcado"
 * usado por los cortes en cascada. La lista de raíces y las listas de hijos son
 * listas doblemente enlazadas circulares (left/right).
 */
struct FibNode {
    int vertex;              // Vértice del grafo que representa.
    double key;              // Clave (costo) asociada al vértice.
    int degree;              // Número de hijos.
    bool flag;               // Marca: perdió un hijo desde la última vez que fue raíz.
    FibNode* parent;         // Padre (null si es raíz).
    FibNode* child;          // Algún hijo (lista circular left/right).
    FibNode* left;           // Predecesor en la lista circular.
    FibNode* right;          // Sucesor en la lista circular.

    FibNode(int v, double k);
};

/**
 * Cola de prioridad implementada con cola de Fibonacci.
 * Soporta insert, extractMin y decreaseKey sobre pares (costo vértice).
 * decreaseKey usa cortes en cascada (O(1) amortizado). node_ptr[v] da acceso
 * directo al nodo de cada vértice.
 */
class FibonacciHeap {
private:
    FibNode* min_node;                  // Nodo de menor clave de la lista de raíces.
    std::vector<FibNode*> node_ptr;     // Acceso directo al nodo de cada vértice.
    int n;                              // Número total de nodos en el heap.

    /**
     * Concatena node a la lista de raíces (justo después de min_node).
     * Pre: la lista de raíces no está vacía.
     */
    void add_to_root_list(FibNode* node);

    /**
     * Hace de y un hijo de x: lo saca de su lista y lo agrega a la de hijos de x.
     * Pre: key(x) <= key(y). Aumenta degree(x) y limpia flag(y).
     */
    void link_nodes(FibNode* y, FibNode* x);

    /**
     * Consolida la lista de raíces: fusiona árboles del mismo grado hasta que
     * todos los grados sean distintos y actualiza min_node.
     */
    void consolidate();

    /** Corta a x (hijo de y): lo mueve a la lista de raíces y decrementa degree(y). */
    void cut(FibNode* x, FibNode* y);

    /** Cortes en cascada: propaga el flag de nodo marcado hacia la raíz. */
    void cascading_cut(FibNode* y);

public:
    long long op_count;     // Número de cortes (cut) ejecutados en decreaseKey.
    long long call_count;   // Número de decreaseKey exitosos (para medición amortizada).

    /** Crea un heap vacío con seguimiento para num_vertices vértices. */
    FibonacciHeap(int num_vertices);
    ~FibonacciHeap();

    /** True si el heap no contiene vértices. */
    bool empty() const;

    /** Inserta el vértice v con clave inicial key. */
    void insert(int v, double key);

    /** Obtiene y elimina el par (vértice, clave) de menor clave. */
    std::pair<int, double> extract_min();

    /** Reduce la clave del vértice v a new_key (debe ser menor que la actual). */
    void decrease_key(int v, double new_key);
};

#endif