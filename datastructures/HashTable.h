#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

namespace SwiftExpedition {  

template<typename K, typename V>
class HashTable {
private:

    unordered_map<K, V> table;

public:
    HashTable() {}

    void insert(const K& key, const V& value) {
        table[key] = value;
    }

    V* find(const K& key) {
        auto it = table.find(key);
        if (it != table.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    bool find(const K& key, V& result) {
        auto it = table.find(key);
        if (it != table.end()) {
            result = it->second;
            return true;
        }
        return false;
    }

    bool remove(const K& key) {
        return table.erase(key) > 0;
    }

    bool contains(const K& key) const {
        return table.find(key) != table.end();
    }

    void display() const {
        if (table.empty()) {
            cout << "  (Hash Table kosong)\n";
            return;
        }
        cout << "  Daftar User (Hash Table):\n";
        for (auto it = table.begin(); it != table.end(); ++it) {
            cout << "  Username: " << it->first
                      << " | Nama: " << it->second.nama
                      << " | Role: " << roleToString(it->second.role) << "\n";
        }
    }

    vector<V> toVector() const {
        vector<V> result;
        for (const auto& pair : table) {
            result.push_back(pair.second);
        }
        return result;
    }

    size_t size() const { return table.size(); }
    bool isEmpty() const { return table.empty(); }
    void clear() { table.clear(); }
};

} 