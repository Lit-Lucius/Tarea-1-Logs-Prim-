#include "FibonacciHeap.hpp"
#include <cmath>
#include <algorithm>

FibNode::FibNode(int v, double k)
    : vertex(v), key(k), degree(0), flag(false), parent(nullptr), child(nullptr) {
    left = this;
    right = this;
}

FibonacciHeap::FibonacciHeap(int num_vertices)
    : min_node(nullptr), node_ptr(num_vertices, nullptr), n(0), op_count(0), call_count(0) {}

FibonacciHeap::~FibonacciHeap() {
    std::vector<FibNode*> stack;
    if (min_node) {
        FibNode* curr = min_node;
        do {
            stack.push_back(curr);
            curr = curr->right;
        } while (curr != min_node);
    }
    while (!stack.empty()) {
        FibNode* curr = stack.back();
        stack.pop_back();

        if (curr->child) {
            FibNode* c = curr->child;
            do {
                stack.push_back(c);
                c = c->right;
            } while (c != curr->child);
        }
        delete curr;
    }
}

void FibonacciHeap::add_to_root_list(FibNode* node) {
    node->left = min_node;
    node->right = min_node->right;
    min_node->right->left = node;
    min_node->right = node;
}

void FibonacciHeap::link_nodes(FibNode* y, FibNode* x) {
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;
    if (!x->child) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->flag = false;
}

void FibonacciHeap::consolidate() {
    int max_deg = static_cast<int>(std::log2(n + 1)) + 10;
    std::vector<FibNode*> A(max_deg, nullptr);

    std::vector<FibNode*> root_nodes;
    if (min_node) {
        FibNode* curr = min_node;
        do {
            root_nodes.push_back(curr);
            curr = curr->right;
        } while (curr != min_node);
    }

    for (FibNode* w : root_nodes) {
        FibNode* x = w;
        int d = x->degree;
        while (A[d] != nullptr) {
            FibNode* y = A[d];
            if (x->key > y->key) std::swap(x, y);
            link_nodes(y, x);
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }

    min_node = nullptr;
    for (int i = 0; i < max_deg; ++i) {
        if (A[i]) {
            if (!min_node) {
                min_node = A[i];
                min_node->left = min_node;
                min_node->right = min_node;
            } else {
                add_to_root_list(A[i]);
                if (A[i]->key < min_node->key) min_node = A[i];
            }
        }
    }
}

void FibonacciHeap::cut(FibNode* x, FibNode* y) {
    if (x->right == x) {
        y->child = nullptr;
    } else {
        if (y->child == x) y->child = x->right;
        x->left->right = x->right;
        x->right->left = x->left;
    }
    y->degree--;

    add_to_root_list(x);
    x->parent = nullptr;
    x->flag = false;
    op_count++;
}

void FibonacciHeap::cascading_cut(FibNode* y) {
    FibNode* z = y->parent;
    if (z) {
        if (!y->flag) {
            y->flag = true;
        } else {
            cut(y, z);
            cascading_cut(z);
        }
    }
}

bool FibonacciHeap::empty() const {
    return min_node == nullptr;
}

void FibonacciHeap::insert(int v, double key) {
    FibNode* node = new FibNode(v, key);
    node_ptr[v] = node;
    if (!min_node) {
        min_node = node;
    } else {
        add_to_root_list(node);
        if (node->key < min_node->key) min_node = node;
    }
    n++;
}

std::pair<int, double> FibonacciHeap::extract_min() {
    FibNode* z = min_node;
    if (z) {
        if (z->child) {
            std::vector<FibNode*> children;
            FibNode* c = z->child;
            do {
                children.push_back(c);
                c = c->right;
            } while (c != z->child);

            for (FibNode* child : children) {
                add_to_root_list(child);
                child->parent = nullptr;
            }
        }

        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right) {
            min_node = nullptr;
        } else {
            min_node = z->right;
            consolidate();
        }
        n--;
        int min_v = z->vertex;
        double min_k = z->key;
        node_ptr[min_v] = nullptr;
        delete z;
        return {min_v, min_k};
    }
    return {-1, -1.0};
}

void FibonacciHeap::decrease_key(int v, double new_key) {
    FibNode* x = node_ptr[v];
    if (!x || new_key >= x->key) return;

    call_count++;
    x->key = new_key;
    FibNode* y = x->parent;

    if (y && x->key < y->key) {
        cut(x, y);
        cascading_cut(y);
    }
    if (x->key < min_node->key) {
        min_node = x;
    }
}