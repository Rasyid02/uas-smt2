#pragma once
#include <iostream>
#include <vector>

// ============================================================
// UAS: Circular Linked List (Struktur Data 2)
// Digunakan untuk ROTASI KURIR
// Kurir diassign paket secara bergiliran (round-robin)
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

template<typename T>
class CircularLinkedList {
private:
    // UAS: struct (1) - Node internal
    struct Node {
        T data;
        Node* next;  // UAS: pointer (*) - 2
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* tail;  // UAS: pointer (*) - 2: Tail untuk circular list
    int size_;

public:
    // UAS: default argument (6)
    CircularLinkedList() : tail(nullptr), size_(0) {}

    ~CircularLinkedList() {
        clear();
    }

    // UAS: inline function (7)
    inline bool isEmpty() const { return tail == nullptr; }
    inline int size() const { return size_; }

    // UAS: reference (&) - 3
    void insert(const T& data) {
        Node* newNode = new Node(data);
        if (!tail) {
            tail = newNode;
            tail->next = tail;  // Circular: menunjuk ke diri sendiri
        } else {
            newNode->next = tail->next;  // newNode menunjuk ke head
            tail->next = newNode;        // tail menunjuk ke newNode
            tail = newNode;              // newNode jadi tail baru
        }
        size_++;
    }

    bool remove(const T& data) {
        if (!tail) return false;

        Node* current = tail->next;
        Node* prev = tail;

        do {
            if (current->data.id == data.id) {
                if (current == tail && current->next == current) {
                    // Satu-satunya node
                    tail = nullptr;
                } else {
                    prev->next = current->next;
                    if (current == tail) {
                        tail = prev;
                    }
                }
                delete current;
                size_--;
                return true;
            }
            prev = current;
            current = current->next;
        } while (current != tail->next);

        return false;
    }

    // UAS: callback function (5) - Mengambil kurir berikutnya dalam rotasi
    T* getNext() {
        if (!tail) return nullptr;
        // Dalam circular, tail->next adalah head (kurir pertama)
        // Setelah diambil, rotasi tail ke depan
        T data = tail->next->data;
        tail = tail->next;
        return &(tail->data);
    }

    // Rotasi ke kurir berikutnya (untuk assign paket secara adil)
    void rotate() {
        if (tail) {
            tail = tail->next;
        }
    }

    // Dapatkan data node saat ini (tail)
    T* current() {
        if (!tail) return nullptr;
        return &(tail->data);
    }

    void display() const {
        if (!tail) {
            std::cout << "  (Tidak ada kurir)\n";
            return;
        }
        Node* temp = tail->next;  // Mulai dari head
        int count = 0;
        do {
            std::cout << "[" << count++ << "] ID: " << temp->data.id
                      << " | Nama: " << temp->data.nama
                      << " | Status: " << temp->data.status << "\n";
            temp = temp->next;
        } while (temp != tail->next);
    }

    std::vector<T> toVector() const {
        std::vector<T> result;
        if (!tail) return result;
        Node* temp = tail->next;
        do {
            result.push_back(temp->data);
            temp = temp->next;
        } while (temp != tail->next);
        return result;
    }

    void clear() {
        if (!tail) return;
        Node* current = tail->next;
        while (current != tail) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        delete tail;
        tail = nullptr;
        size_ = 0;
    }
};

} // namespace SwiftExpedition