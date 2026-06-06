#pragma once
#include <iostream>
#include <vector>
#include <functional>

// ============================================================
// UAS: Singly Linked List (Struktur Data 1)
// Digunakan untuk MASTER DATA PAKET
// Implementasi penuh dengan Node, insert, delete, update, search
// Dilengkapi iterator (UAS: iterator - 11)
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

template<typename T>
class SinglyLinkedList {
private:
    // UAS: struct (1) - Node internal untuk linked list
    struct Node {
        T data;
        Node* next;  // UAS: pointer (*) - 2
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;  // UAS: pointer (*) - 2
    int size_;

public:
    // ============================================================
    // UAS: Iterator (11) - Iterator class untuk traversal linked list
    // ============================================================
    class Iterator {
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}

        T& operator*() { return current->data; }
        T* operator->() { return &(current->data); }

        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    // ============================================================
    // UAS: default argument (6) - Konstruktor dengan nilai default
    // ============================================================
    SinglyLinkedList() : head(nullptr), size_(0) {}

    ~SinglyLinkedList() {
        clear();
    }

    // UAS: inline function (7) - Fungsi sederhana di header
    inline bool isEmpty() const { return head == nullptr; }
    inline int size() const { return size_; }

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    // UAS: reference (&) - 3: Parameter data menggunakan const reference
    void insert(const T& data) {
        Node* newNode = new Node(data);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size_++;
    }

    // UAS: function overloading (8) - Insert di awal (prepend)
    void insertAtFront(const T& data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
        size_++;
    }

    // UAS: function overloading (8) - Insert di posisi tertentu
    bool insertAt(int index, const T& data) {
        if (index < 0 || index > size_) return false;
        if (index == 0) {
            insertAtFront(data);
            return true;
        }
        Node* newNode = new Node(data);
        Node* temp = head;
        for (int i = 0; i < index - 1; i++) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
        size_++;
        return true;
    }

    bool remove(const T& data) {
        if (!head) return false;

        if (head->data.id == data.id) {
            Node* toDelete = head;
            head = head->next;
            delete toDelete;
            size_--;
            return true;
        }

        Node* temp = head;
        while (temp->next != nullptr && temp->next->data.id != data.id) {
            temp = temp->next;
        }

        if (temp->next == nullptr) return false;

        Node* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
        size_--;
        return true;
    }

    bool removeById(int id) {
        if (!head) return false;

        if (head->data.id == id) {
            Node* toDelete = head;
            head = head->next;
            delete toDelete;
            size_--;
            return true;
        }

        Node* temp = head;
        while (temp->next != nullptr && temp->next->data.id != id) {
            temp = temp->next;
        }

        if (temp->next == nullptr) return false;

        Node* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
        size_--;
        return true;
    }

    // UAS: pointer (*) - 2: Mencari data, return pointer ke data atau nullptr
    T* findById(int id) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                return &(temp->data);  // UAS: pointer (*) return
            }
            temp = temp->next;
        }
        return nullptr;
    }

    // UAS: reference (&) - 3: Reference parameter untuk output
    bool findById(int id, T& result) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                result = temp->data;
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // Cari berdasarkan resi (string)
    T* findByResi(const std::string& resi) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data.resi == resi) {
                return &(temp->data);
            }
            temp = temp->next;
        }
        return nullptr;
    }

    bool update(int id, const T& newData) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                temp->data = newData;
                temp->data.id = id;  // Pertahankan ID
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // UAS: callback function (5) - Traversal dengan callback
    void forEach(std::function<void(T&)> callback) {
        Node* temp = head;
        while (temp != nullptr) {
            callback(temp->data);
            temp = temp->next;
        }
    }

    // Konversi ke vector (untuk sorting dan display)
    std::vector<T> toVector() const {
        std::vector<T> result;
        Node* temp = head;
        while (temp != nullptr) {
            result.push_back(temp->data);
            temp = temp->next;
        }
        return result;
    }

    void display() const {
        Node* temp = head;
        int count = 0;
        while (temp != nullptr) {
            std::cout << "[" << count++ << "] ID: " << temp->data.id
                      << " | Resi: " << temp->data.resi
                      << " | Penerima: " << temp->data.nama_penerima
                      << " | Status: " << temp->data.status << "\n";
            temp = temp->next;
        }
        if (count == 0) std::cout << "  (Tidak ada data)\n";
    }

    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size_ = 0;
    }

    // Mendapatkan jumlah node (digunakan untuk count)
    int countByStatus(const std::string& status) const {
        int count = 0;
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data.status == status) count++;
            temp = temp->next;
        }
        return count;
    }
};

} // namespace SwiftExpedition