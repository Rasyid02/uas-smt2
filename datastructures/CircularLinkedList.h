#pragma once
#include <iostream>
#include <vector>
using namespace std;

namespace SwiftExpedition {  

template<typename T>
class CircularLinkedList {
private:

    struct Node {
        T data;
        Node* next;  
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* tail;  
    int size_;

public:

    CircularLinkedList() : tail(nullptr), size_(0) {}

    ~CircularLinkedList() {
        clear();
    }

    inline bool isEmpty() const { return tail == nullptr; }
    inline int size() const { return size_; }

    void insert(const T& data) {
        Node* newNode = new Node(data);
        if (!tail) {
            tail = newNode;
            tail->next = tail;  
        } else {
            newNode->next = tail->next;  
            tail->next = newNode;        
            tail = newNode;              
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

    T* getNext() {
        if (!tail) return nullptr;

        T data = tail->next->data;
        tail = tail->next;
        return &(tail->data);
    }

    void rotate() {
        if (tail) {
            tail = tail->next;
        }
    }

    T* current() {
        if (!tail) return nullptr;
        return &(tail->data);
    }

    void display() const {
        if (!tail) {
            cout << "  (Tidak ada kurir)\n";
            return;
        }
        Node* temp = tail->next;  
        int count = 0;
        do {
            cout << "[" << count++ << "] ID: " << temp->data.id
                      << " | Nama: " << temp->data.nama
                      << " | Status: " << temp->data.status << "\n";
            temp = temp->next;
        } while (temp != tail->next);
    }

    vector<T> toVector() const {
        vector<T> result;
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

} 