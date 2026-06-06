#pragma once
#include <iostream>
#include <vector>
using namespace std;

namespace SwiftExpedition {  

template<typename T>
class Queue {
private:

    struct Node {
        T data;
        Node* next;  
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* frontNode;  
    Node* rearNode;   
    int size_;

public:

    Queue() : frontNode(nullptr), rearNode(nullptr), size_(0) {}

    ~Queue() {
        clear();
    }

    inline bool isEmpty() const { return frontNode == nullptr; }
    inline int size() const { return size_; }

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

    T dequeue() {
        if (!frontNode) {
            throw runtime_error("Queue kosong! Tidak bisa dequeue.");
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

    T& front() {
        if (!frontNode) {
            throw runtime_error("Queue kosong! Tidak bisa front.");
        }
        return frontNode->data;
    }

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

    vector<T> toVector() const {
        vector<T> result;
        Node* temp = frontNode;
        while (temp != nullptr) {
            result.push_back(temp->data);
            temp = temp->next;
        }
        return result;
    }

    void display() const {
        if (!frontNode) {
            cout << "  (Antrean kosong)\n";
            return;
        }
        cout << "  Antrean Paket Masuk:\n";
        Node* temp = frontNode;
        int count = 1;
        while (temp != nullptr) {
            cout << "  [" << count++ << "] Resi: " << temp->data.resi
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

} 