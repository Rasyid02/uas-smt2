#pragma once
#include <iostream>
#include <vector>

// ============================================================
// UAS: Stack (Struktur Data 3)
// Digunakan untuk UNDO STATUS TRACKING
// Menyimpan history status tracking, bisa di-pop untuk undo
// Implementasi LIFO (Last In, First Out)
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

template<typename T>
class Stack {
private:
    // UAS: struct (1) - Node internal
    struct Node {
        T data;
        Node* next;  // UAS: pointer (*) - 2
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* topNode;  // UAS: pointer (*) - 2
    int size_;

public:
    // UAS: default argument (6)
    Stack() : topNode(nullptr), size_(0) {}

    ~Stack() {
        clear();
    }

    // UAS: inline function (7)
    inline bool isEmpty() const { return topNode == nullptr; }
    inline int size() const { return size_; }

    // UAS: reference (&) - 3
    void push(const T& data) {
        Node* newNode = new Node(data);
        newNode->next = topNode;
        topNode = newNode;
        size_++;
    }

    // Pop: menghapus dan mengembalikan data teratas
    // UAS: exception handling (9) - throw jika stack kosong
    T pop() {
        if (!topNode) {
            throw std::runtime_error("Stack kosong! Tidak bisa pop.");
        }
        Node* temp = topNode;
        T data = temp->data;
        topNode = topNode->next;
        delete temp;
        size_--;
        return data;
    }

    // Peek: melihat data teratas tanpa menghapus
    T& top() {
        if (!topNode) {
            throw std::runtime_error("Stack kosong! Tidak bisa top.");
        }
        return topNode->data;
    }

    // Pop tanpa exception (untuk undo tracking)
    bool pop(T& result) {
        if (!topNode) return false;
        Node* temp = topNode;
        result = temp->data;
        topNode = topNode->next;
        delete temp;
        size_--;
        return true;
    }

    std::vector<T> toVector() const {
        std::vector<T> result;
        Node* temp = topNode;
        while (temp != nullptr) {
            result.push_back(temp->data);
            temp = temp->next;
        }
        return result;
    }

    void display() const {
        if (!topNode) {
            std::cout << "  (Stack kosong)\n";
            return;
        }
        std::cout << "  Stack Tracking History (terbaru di atas):\n";
        Node* temp = topNode;
        int count = 0;
        while (temp != nullptr) {
            std::cout << "  [" << count++ << "] " << temp->data.status
                      << " - " << temp->data.lokasi << " (" << temp->data.timestamp << ")\n";
            temp = temp->next;
        }
    }

    void clear() {
        while (topNode != nullptr) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
        size_ = 0;
    }
};

} // namespace SwiftExpedition