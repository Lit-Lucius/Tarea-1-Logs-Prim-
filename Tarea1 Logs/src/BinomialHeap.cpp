#include "BinomialHeap.hpp"
#include <algorithm>

BinomialNode::BinomialNode(int v, double k)
    : vertex(v), key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}

BinomialHeap::BinomialHeap(int num_vertices)
    : head(nullptr), node_ptr(num_vertices, nullptr), op_count(0), call_count(0) {}

BinomialHeap::~BinomialHeap() {
    std::vector<BinomialNode*> stack;
    if (head) stack.push_back(head);
    while (!stack.empty()) {
        BinomialNode* curr = stack.back();
        stack.pop_back();
        if (curr->sibling) stack.push_back(curr->sibling);
        if (curr->child) stack.push_back(curr->child);
        delete curr;
    }
}

void BinomialHeap::link_trees(BinomialNode* min_root, BinomialNode* other_root) {
    other_root->parent = min_root;
    other_root->sibling = min_root->child;
    min_root->child = other_root;
    min_root->degree++;
}

BinomialNode* BinomialHeap::merge_roots(BinomialNode* h1, BinomialNode* h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    BinomialNode* res_head = nullptr;
    BinomialNode** tail = &res_head;

    while (h1 && h2) {
        if (h1->degree <= h2->degree) {
            *tail = h1;
            h1 = h1->sibling;
        } else {
            *tail = h2;
            h2 = h2->sibling;
        }
        tail = &((*tail)->sibling);
    }
    *tail = (h1) ? h1 : h2;
    return res_head;
}

void BinomialHeap::union_heap(BinomialNode* h2_head) {
    this->head = merge_roots(this->head, h2_head);
    if (!this->head) return;

    BinomialNode* prev = nullptr;
    BinomialNode* curr = this->head;
    BinomialNode* next = curr->sibling;

    while (next) {
        if ((curr->degree != next->degree) ||
            (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else {
            if (curr->key <= next->key) {
                curr->sibling = next->sibling;
                link_trees(curr, next);
            } else {
                if (!prev) this->head = next;
                else prev->sibling = next;
                link_trees(next, curr);
                curr = next;
            }
        }
        next = curr->sibling;
    }
}

bool BinomialHeap::empty() const {
    return head == nullptr;
}

void BinomialHeap::insert(int v, double key) {
    BinomialNode* node = new BinomialNode(v, key);
    node_ptr[v] = node;
    union_heap(node);
}

std::pair<int, double> BinomialHeap::extract_min() {
    if (!head) return {-1, -1.0};

    BinomialNode* min_node = head;
    BinomialNode* min_prev = nullptr;
    BinomialNode* prev = head;
    BinomialNode* curr = head->sibling;

    while (curr) {
        if (curr->key < min_node->key) {
            min_node = curr;
            min_prev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }

    if (min_prev) min_prev->sibling = min_node->sibling;
    else head = min_node->sibling;

    BinomialNode* child_head = nullptr;
    curr = min_node->child;
    while (curr) {
        BinomialNode* next_child = curr->sibling;
        curr->sibling = child_head;
        curr->parent = nullptr;
        child_head = curr;
        curr = next_child;
    }

    if (child_head) {
        union_heap(child_head);
    }

    int min_v = min_node->vertex;
    double min_k = min_node->key;
    node_ptr[min_v] = nullptr;
    delete min_node;

    return {min_v, min_k};
}

void BinomialHeap::decrease_key(int v, double new_key) {
    BinomialNode* x = node_ptr[v];
    if (!x || new_key >= x->key) return;

    call_count++;
    x->key = new_key;
    BinomialNode* y = x->parent;

    while (y && x->key < y->key) {
        op_count++;
        std::swap(x->key, y->key);
        std::swap(x->vertex, y->vertex);

        node_ptr[x->vertex] = x;
        node_ptr[y->vertex] = y;

        x = y;
        y = x->parent;
    }
}