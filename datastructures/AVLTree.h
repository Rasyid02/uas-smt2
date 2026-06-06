#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// ============================================================
// UAS: AVL Tree (Struktur Data 5)
// Self-balancing Binary Search Tree
// Digunakan untuk PENCARIAN NOMOR RESI secara cepat
// Key: nomor resi (string), Value: ID paket (int)
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

template<typename K, typename V>
class AVLTree {
private:
    // UAS: struct (1) - Node internal AVL Tree
    struct Node {
        K key;
        V value;
        Node* left;   // UAS: pointer (*) - 2
        Node* right;  // UAS: pointer (*) - 2
        int height;

        Node(const K& k, const V& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;  // UAS: pointer (*) - 2

    // UAS: inline function (7) - Utility height
    inline int height(Node* node) const {
        return node ? node->height : 0;
    }

    inline int getBalance(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // Rotasi AVL
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    Node* insertNode(Node* node, const K& key, const V& value) {
        if (!node) return new Node(key, value);

        if (key < node->key)
            node->left = insertNode(node->left, key, value);
        else if (key > node->key)
            node->right = insertNode(node->right, key, value);
        else {
            // Key sudah ada, update value
            node->value = value;
            return node;
        }

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);

        // Left Left
        if (balance > 1 && key < node->left->key)
            return rotateRight(node);
        // Right Right
        if (balance < -1 && key > node->right->key)
            return rotateLeft(node);
        // Left Right
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // Right Left
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // UAS: pointer (*) - 2: Search mengembalikan pointer ke value
    V* searchNode(Node* node, const K& key) {
        if (!node) return nullptr;
        if (key == node->key) return &(node->value);
        if (key < node->key) return searchNode(node->left, key);
        return searchNode(node->right, key);
    }

    void inorderTraversal(Node* node, std::vector<std::pair<K, V>>& result) const {
        if (!node) return;
        inorderTraversal(node->left, result);
        result.push_back({node->key, node->value});
        inorderTraversal(node->right, result);
    }

    void clearNode(Node* node) {
        if (!node) return;
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clear();
    }

    // UAS: reference (&) - 3
    void insert(const K& key, const V& value) {
        root = insertNode(root, key, value);
    }

    // UAS: pointer (*) - 2: Search return pointer
    V* search(const K& key) {
        return searchNode(root, key);
    }

    bool search(const K& key, V& result) {
        V* found = searchNode(root, key);
        if (found) {
            result = *found;
            return true;
        }
        return false;
    }

    void display() const {
        std::vector<std::pair<K, V>> result;
        inorderTraversal(root, result);
        if (result.empty()) {
            std::cout << "  (AVL Tree kosong)\n";
            return;
        }
        std::cout << "  Daftar Resi (AVL Tree - inorder):\n";
        for (size_t i = 0; i < result.size(); i++) {
            std::cout << "  [" << i << "] Resi: " << result[i].first
                      << " -> ID Paket: " << result[i].second << "\n";
        }
    }

    std::vector<std::pair<K, V>> toVector() const {
        std::vector<std::pair<K, V>> result;
        inorderTraversal(root, result);
        return result;
    }

    void clear() {
        clearNode(root);
        root = nullptr;
    }

    bool isEmpty() const { return root == nullptr; }
};

} // namespace SwiftExpedition