#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// ============================================================
// UAS: Hash Table (Struktur Data 9)
// Menggunakan STL unordered_map untuk LOGIN USER
// Key: username (string), Value: User
// Akses O(1) average case untuk validasi login
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

template<typename K, typename V>
class HashTable {
private:
    // UAS: STL unordered_map (10) - Implementasi hash table
    std::unordered_map<K, V> table;

public:
    HashTable() {}

    // UAS: reference (&) - 3
    void insert(const K& key, const V& value) {
        table[key] = value;
    }

    // UAS: pointer (*) - 2: Search return pointer
    V* find(const K& key) {
        auto it = table.find(key);
        if (it != table.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    // UAS: reference (&) - 3: Search dengan reference parameter
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

    // UAS: iterator (11) - Iterator dari STL unordered_map
    void display() const {
        if (table.empty()) {
            std::cout << "  (Hash Table kosong)\n";
            return;
        }
        std::cout << "  Daftar User (Hash Table):\n";
        for (auto it = table.begin(); it != table.end(); ++it) {
            std::cout << "  Username: " << it->first
                      << " | Nama: " << it->second.nama
                      << " | Role: " << roleToString(it->second.role) << "\n";
        }
    }

    std::vector<V> toVector() const {
        std::vector<V> result;
        for (const auto& pair : table) {
            result.push_back(pair.second);
        }
        return result;
    }

    size_t size() const { return table.size(); }
    bool isEmpty() const { return table.empty(); }
    void clear() { table.clear(); }
};

} // namespace SwiftExpedition