#pragma once
#include <iostream>
#include <vector>

// ============================================================
// UAS: Queue (Struktur Data 4)
// Digunakan untuk ANTREAN PAKET MASUK
// Paket yang baru dibuat masuk ke queue, lalu diambil kurir
// Implementasi FIFO (First In, First Out)
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

template<typename T>
class Queue {
private:
    // UAS: struct (1) - Node internal
    struct Node {
        T data;
        Node* next;  // UAS: pointer (*) - 2
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* frontNode;  // UAS: pointer (*) - 2
    Node* rearNode;   // UAS: pointer (*) - 2
    int size_;

public:
    // UAS: default argument (6)
    Queue() : frontNode(nullptr), rearNode(nullptr), size_(0) {}

    ~Queue() {
        clear();
    }

    // UAS: inline function (7)
    inline bool isEmpty() const { return frontNode == nullptr; }
    inline int size() const { return size_; }

    // UAS: reference (&) - 3
    void enqueue(const T& data) {
        Node* newNode = new Node(data);
        if (!rearNode) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        size_++;
    }

    // Dequeue: mengambil data dari depan antrean
    // UAS: exception handling (9) - throw jika queue kosong
    T dequeue() {
        if (!frontNode) {
            throw std::runtime_error("Queue kosong! Tidak bisa dequeue.");
        }
        Node* temp = frontNode;
        T data = temp->data;
        frontNode = frontNode->next;
        if (!frontNode) {
            rearNode = nullptr;
        }
        delete temp;
        size_--;
        return data;
    }

    // Peek front tanpa menghapus
    T& front() {
        if (!frontNode) {
            throw std::runtime_error("Queue kosong! Tidak bisa front.");
        }
        return frontNode->data;
    }

    // Dequeue tanpa exception
    bool dequeue(T& result) {
        if (!frontNode) return false;
        Node* temp = frontNode;
        result = temp->data;
        frontNode = frontNode->next;
        if (!frontNode) {
            rearNode = nullptr;
        }
        delete temp;
        size_--;
        return true;
    }

    std::vector<T> toVector() const {
        std::vector<T> result;
        Node* temp = frontNode;
        while (temp != nullptr) {
            result.push_back(temp->data);
            temp = temp->next;
        }
        return result;
    }

    void display() const {
        if (!frontNode) {
            std::cout << "  (Antrean kosong)\n";
            return;
        }
        std::cout << "  Antrean Paket Masuk:\n";
        Node* temp = frontNode;
        int count = 1;
        while (temp != nullptr) {
            std::cout << "  [" << count++ << "] Resi: " << temp->data.resi
                      << " | Penerima: " << temp->data.nama_penerima
                      << " | Tujuan: " << temp->data.kota_tujuan << "\n";
            temp = temp->next;
        }
    }

    void clear() {
        while (frontNode != nullptr) {
            Node* temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }
        rearNode = nullptr;
        size_ = 0;
    }
};

} // namespace SwiftExpedition