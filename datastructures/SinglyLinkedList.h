#pragma once
#include <iostream>
#include <vector>
#include <functional>
using namespace std;

namespace SwiftExpedition {  

template<typename T>
class SinglyLinkedList {
private:

    struct Node {
        T data;
        Node* next;  
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;  
    int size_;

public:

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

    SinglyLinkedList() : head(nullptr), size_(0) {}

    ~SinglyLinkedList() {
        clear();
    }

    inline bool isEmpty() const { return head == nullptr; }
    inline int size() const { return size_; }

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

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

    void insertAtFront(const T& data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
        size_++;
    }

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

    T* findById(int id) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                return &(temp->data);  
            }
            temp = temp->next;
        }
        return nullptr;
    }

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

    T* findByResi(const string& resi) {
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
                temp->data.id = id;  
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    void forEach(function<void(T&)> callback) {
        Node* temp = head;
        while (temp != nullptr) {
            callback(temp->data);
            temp = temp->next;
        }
    }

    vector<T> toVector() const {
        vector<T> result;
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
            cout << "[" << count++ << "] ID: " << temp->data.id
                      << " | Resi: " << temp->data.resi
                      << " | Penerima: " << temp->data.nama_penerima
                      << " | Status: " << temp->data.status << "\n";
            temp = temp->next;
        }
        if (count == 0) cout << "  (Tidak ada data)\n";
    }

    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size_ = 0;
    }

    int countByStatus(const string& status) const {
        int count = 0;
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data.status == status) count++;
            temp = temp->next;
        }
        return count;
    }
};

} 