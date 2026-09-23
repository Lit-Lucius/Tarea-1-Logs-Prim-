#ifndef BINOMIAL_HEAP_HPP
#define BINOMIAL_HEAP_HPP

#include <vector>
#include <utility>

/**
 * Nodo de un arbol binomial B_k.
 * Guarda el vertice del grafo que representa, su clave (costo) y los punteros
 * de arbol (parent, child mas a la izquierda, sibling a la derecha).
 */
struct BinomialNode {
    int vertex;                  // Vértice del grafo que representa este nodo.
    double key;                  // Clave (costo) asociada al vértice.
    int degree;                  // Grado del árbol binomial (cantidad de hijos).
    BinomialNode* parent;        // Padre en el árbol binomial (null si es raíz).
    BinomialNode* child;         // Hijo más "grande" (mayor grado).
    BinomialNode* sibling;       // Siguiente raíz/hijo en la lista de hermanos.

    BinomialNode(int v, double k);
};

/**
 * Cola de prioridad implementada con cola binomial.
 * Soporta insert, extractMin y decreaseKey sobre pares (costo, vértice).
 * Se mantiene node_ptr[v] para acceder en O(1) al nodo del heap de cada vértice.
 */
class BinomialHeap {
private:
    BinomialNode* head;                     // Primera raíz de la lista de árboles.
    std::vector<BinomialNode*> node_ptr;    // Acceso directo al nodo de cada vértice.

    /**
     * Hace de other_root el hijo más grande de min_root (misma B_k), uniendo los árboles.
     * Pre: ambos tienen el mismo grado; min_root tiene clave <= other_root.
     */
    void link_trees(BinomialNode* min_root, BinomialNode* other_root);

    /**
     * Fusiona dos listas de raíces ordenadas por grado (sin combinar grados iguales).
     * Devuelve el nuevo head. No modifica n.
     */
    BinomialNode* merge_roots(BinomialNode* h1, BinomialNode* h2);

    /**
     * Une el heap actual con la lista de árboles h2_head y reordena por grado.
     */
    void union_heap(BinomialNode* h2_head);

public:
    long long op_count;     // Número de intercambios (swaps) realizados en decreaseKey.
    long long call_count;   // Número de decreaseKey exitosos (para medición amortizada).

    /** Crea un heap vacío con capacidad de seguimiento para num_vertices vértices. */
    BinomialHeap(int num_vertices);
    ~BinomialHeap();

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