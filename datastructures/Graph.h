#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>

// ============================================================
// UAS: Graph (Struktur Data 6)
// Representasi jalur antar kota menggunakan adjacency list
// UAS: BFS (Struktur Data 7) - Mencari jalur transit terdekat
// UAS: DFS (Struktur Data 8) - Menampilkan seluruh jalur
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

struct Edge {
    std::string tujuan;
    int jarak;  // Jarak dalam km
};

class Graph {
private:
    // UAS: STL map + vector (10) - Adjacency list
    std::map<std::string, std::vector<Edge>> adjList;

public:
    Graph() {}

    // UAS: reference (&) - 3
    void addEdge(const std::string& asal, const std::string& tujuan, int jarak) {
        adjList[asal].push_back({tujuan, jarak});
        adjList[tujuan].push_back({asal, jarak});  // Undirected graph
    }

    void display() const {
        std::cout << "\n  ===== REPRESENTASI GRAF JALUR ANTAR KOTA =====\n";
        for (const auto& pair : adjList) {
            std::cout << "  " << pair.first << " ->";
            for (size_t i = 0; i < pair.second.size(); i++) {
                std::cout << " " << pair.second[i].tujuan
                          << "(" << pair.second[i].jarak << "km)";
                if (i < pair.second.size() - 1) std::cout << ",";
            }
            std::cout << "\n";
        }
    }

    // ============================================================
    // UAS: BFS (Struktur Data 7)
    // Mencari jalur transit terdekat menggunakan Breadth First Search
    // Dari kota asal ke kota tujuan, mencari rute dengan transit paling sedikit
    // ============================================================
    std::vector<std::string> BFS(const std::string& asal, const std::string& tujuan) {
        std::map<std::string, std::string> parent;
        std::map<std::string, bool> visited;
        std::queue<std::string> q;

        // Inisialisasi
        for (const auto& pair : adjList) {
            visited[pair.first] = false;
        }

        visited[asal] = true;
        q.push(asal);
        parent[asal] = "";

        bool found = false;

        while (!q.empty() && !found) {
            std::string current = q.front();
            q.pop();

            // UAS: lambda expression (16) - Filter node yang belum dikunjungi
            auto isUnvisited = [&visited](const std::string& node) {
                return !visited[node];
            };

            for (const Edge& edge : adjList[current]) {
                if (isUnvisited(edge.tujuan)) {
                    visited[edge.tujuan] = true;
                    parent[edge.tujuan] = current;
                    q.push(edge.tujuan);

                    if (edge.tujuan == tujuan) {
                        found = true;
                        break;
                    }
                }
            }
        }

        // Rekonstruksi jalur
        std::vector<std::string> path;
        if (!found) return path;

        std::string current = tujuan;
        while (current != "") {
            path.push_back(current);
            current = parent[current];
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    // ============================================================
    // UAS: DFS (Struktur Data 8)
    // Menampilkan seluruh jalur yang dapat ditempuh dari suatu kota
    // Menggunakan Depth First Search
    // ============================================================
    void DFS(const std::string& start) {
        std::map<std::string, bool> visited;
        for (const auto& pair : adjList) {
            visited[pair.first] = false;
        }

        std::cout << "\n  ===== DFS: SEMUA JALUR DARI " << start << " =====\n";
        std::cout << "  Urutan kunjungan DFS: ";
        DFSRecursive(start, visited);
        std::cout << "\n";
    }

    void DFSRecursive(const std::string& node, std::map<std::string, bool>& visited) {
        visited[node] = true;
        std::cout << node << " -> ";

        for (const Edge& edge : adjList[node]) {
            if (!visited[edge.tujuan]) {
                DFSRecursive(edge.tujuan, visited);
            }
        }
    }

    // Mencari semua jalur dari asal ke tujuan (DFS all paths)
    void findAllPaths(const std::string& asal, const std::string& tujuan) {
        std::map<std::string, bool> visited;
        for (const auto& pair : adjList) {
            visited[pair.first] = false;
        }

        std::vector<std::string> currentPath;
        int pathCount = 0;

        std::cout << "\n  ===== SEMUA JALUR DARI " << asal
                  << " KE " << tujuan << " =====\n";
        findAllPathsDFS(asal, tujuan, visited, currentPath, pathCount);

        if (pathCount == 0) {
            std::cout << "  Tidak ada jalur yang ditemukan.\n";
        }
    }

    void findAllPathsDFS(const std::string& current,
                         const std::string& tujuan,
                         std::map<std::string, bool>& visited,
                         std::vector<std::string>& currentPath,
                         int& pathCount) {
        visited[current] = true;
        currentPath.push_back(current);

        if (current == tujuan) {
            pathCount++;
            std::cout << "  Jalur " << pathCount << ": ";
            int totalJarak = 0;
            for (size_t i = 0; i < currentPath.size(); i++) {
                std::cout << currentPath[i];
                if (i < currentPath.size() - 1) {
                    // Cari jarak antar node
                    for (const Edge& e : adjList[currentPath[i]]) {
                        if (e.tujuan == currentPath[i + 1]) {
                            totalJarak += e.jarak;
                            std::cout << " --(" << e.jarak << "km)--> ";
                            break;
                        }
                    }
                }
            }
            std::cout << " | Total Jarak: " << totalJarak << " km\n";
        } else {
            for (const Edge& edge : adjList[current]) {
                if (!visited[edge.tujuan]) {
                    findAllPathsDFS(edge.tujuan, tujuan, visited, currentPath, pathCount);
                }
            }
        }

        currentPath.pop_back();
        visited[current] = false;
    }

    // Mendapatkan total jarak BFS
    int getBFSDistance(const std::string& asal, const std::string& tujuan) {
        std::vector<std::string> path = BFS(asal, tujuan);
        if (path.empty()) return -1;

        int total = 0;
        for (size_t i = 0; i < path.size() - 1; i++) {
            for (const Edge& e : adjList[path[i]]) {
                if (e.tujuan == path[i + 1]) {
                    total += e.jarak;
                    break;
                }
            }
        }
        return total;
    }

    bool hasCity(const std::string& city) const {
        return adjList.find(city) != adjList.end();
    }

    std::vector<std::string> getCities() const {
        std::vector<std::string> cities;
        for (const auto& pair : adjList) {
            cities.push_back(pair.first);
        }
        return cities;
    }
};

} // namespace SwiftExpedition