#pragma once
#include <iostream>
#include <vector>
using namespace std;

namespace SwiftExpedition {  

template<typename T>
class Stack {
private:

    struct Node {
        T data;
        Node* next;  
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* topNode;  
    int size_;

public:

    Stack() : topNode(nullptr), size_(0) {}

    ~Stack() {
        clear();
    }

    inline bool isEmpty() const { return topNode == nullptr; }
    inline int size() const { return size_; }

    void push(const T& data) {
        Node* newNode = new Node(data);
        newNode->next = topNode;
        topNode = newNode;
        size_++;
    }

    T pop() {
        if (!topNode) {
            throw runtime_error("Stack kosong! Tidak bisa pop.");
        }
        Node* temp = topNode;
        T data = temp->data;
        topNode = topNode->next;
        delete temp;
        size_--;
        return data;
    }

    T& top() {
        if (!topNode) {
            throw runtime_error("Stack kosong! Tidak bisa top.");
        }
        return topNode->data;
    }

    bool pop(T& result) {
        if (!topNode) return false;
        Node* temp = topNode;
        result = temp->data;
        topNode = topNode->next;
        delete temp;
        size_--;
        return true;
    }

    vector<T> toVector() const {
        vector<T> result;
        Node* temp = topNode;
        while (temp != nullptr) {
            result.push_back(temp->data);
            temp = temp->next;
        }
        return result;
    }

    void display() const {
        if (!topNode) {
            cout << "  (Stack kosong)\n";
            return;
        }
        cout << "  Stack Tracking History (terbaru di atas):\n";
        Node* temp = topNode;
        int count = 0;
        while (temp != nullptr) {
            cout << "  [" << count++ << "] " << temp->data.status
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

} 