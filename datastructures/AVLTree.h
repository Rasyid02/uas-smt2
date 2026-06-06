#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

namespace SwiftExpedition {  

template<typename K, typename V>
class AVLTree {
private:

    struct Node {
        K key;
        V value;
        Node* left;   
        Node* right;  
        int height;

        Node(const K& k, const V& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;  

    inline int height(Node* node) const {
        return node ? node->height : 0;
    }

    inline int getBalance(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    Node* insertNode(Node* node, const K& key, const V& value) {
        if (!node) return new Node(key, value);

        if (key < node->key)
            node->left = insertNode(node->left, key, value);
        else if (key > node->key)
            node->right = insertNode(node->right, key, value);
        else {

            node->value = value;
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key)
            return rotateRight(node);

        if (balance < -1 && key > node->right->key)
            return rotateLeft(node);

        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    V* searchNode(Node* node, const K& key) {
        if (!node) return nullptr;
        if (key == node->key) return &(node->value);
        if (key < node->key) return searchNode(node->left, key);
        return searchNode(node->right, key);
    }

    void inorderTraversal(Node* node, vector<pair<K, V>>& result) const {
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

    void insert(const K& key, const V& value) {
        root = insertNode(root, key, value);
    }

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
        vector<pair<K, V>> result;
        inorderTraversal(root, result);
        if (result.empty()) {
            cout << "  (AVL Tree kosong)\n";
            return;
        }
        cout << "  Daftar Resi (AVL Tree - inorder):\n";
        for (size_t i = 0; i < result.size(); i++) {
            cout << "  [" << i << "] Resi: " << result[i].first
                      << " -> ID Paket: " << result[i].second << "\n";
        }
    }

    vector<pair<K, V>> toVector() const {
        vector<pair<K, V>> result;
        inorderTraversal(root, result);
        return result;
    }

    void clear() {
        clearNode(root);
        root = nullptr;
    }

    bool isEmpty() const { return root == nullptr; }
};

} 