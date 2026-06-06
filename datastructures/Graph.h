#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
using namespace std;

namespace SwiftExpedition {  

struct Edge {
    string tujuan;
    int jarak;  
};

class Graph {
private:

    map<string, vector<Edge>> adjList;

public:
    Graph() {}

    void addEdge(const string& asal, const string& tujuan, int jarak) {
        adjList[asal].push_back({tujuan, jarak});
        adjList[tujuan].push_back({asal, jarak});  
    }

    void display() const {
        cout << "\n  ===== REPRESENTASI GRAF JALUR ANTAR KOTA =====\n";
        for (const auto& pair : adjList) {
            cout << "  " << pair.first << " ->";
            for (size_t i = 0; i < pair.second.size(); i++) {
                cout << " " << pair.second[i].tujuan
                          << "(" << pair.second[i].jarak << "km)";
                if (i < pair.second.size() - 1) cout << ",";
            }
            cout << "\n";
        }
    }

    vector<string> BFS(const string& asal, const string& tujuan) {
        map<string, string> parent;
        map<string, bool> visited;
        queue<string> q;

        for (const auto& pair : adjList) {
            visited[pair.first] = false;
        }

        visited[asal] = true;
        q.push(asal);
        parent[asal] = "";

        bool found = false;

        while (!q.empty() && !found) {
            string current = q.front();
            q.pop();

            auto isUnvisited = [&visited](const string& node) {
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

        vector<string> path;
        if (!found) return path;

        string current = tujuan;
        while (current != "") {
            path.push_back(current);
            current = parent[current];
        }
        reverse(path.begin(), path.end());
        return path;
    }

    void DFS(const string& start) {
        map<string, bool> visited;
        for (const auto& pair : adjList) {
            visited[pair.first] = false;
        }

        cout << "\n  ===== DFS: SEMUA JALUR DARI " << start << " =====\n";
        cout << "  Urutan kunjungan DFS: ";
        DFSRecursive(start, visited);
        cout << "\n";
    }

    void DFSRecursive(const string& node, map<string, bool>& visited) {
        visited[node] = true;
        cout << node << " -> ";

        for (const Edge& edge : adjList[node]) {
            if (!visited[edge.tujuan]) {
                DFSRecursive(edge.tujuan, visited);
            }
        }
    }

    void findAllPaths(const string& asal, const string& tujuan) {
        map<string, bool> visited;
        for (const auto& pair : adjList) {
            visited[pair.first] = false;
        }

        vector<string> currentPath;
        int pathCount = 0;

        cout << "\n  ===== SEMUA JALUR DARI " << asal
                  << " KE " << tujuan << " =====\n";
        findAllPathsDFS(asal, tujuan, visited, currentPath, pathCount);

        if (pathCount == 0) {
            cout << "  Tidak ada jalur yang ditemukan.\n";
        }
    }

    void findAllPathsDFS(const string& current,
                         const string& tujuan,
                         map<string, bool>& visited,
                         vector<string>& currentPath,
                         int& pathCount) {
        visited[current] = true;
        currentPath.push_back(current);

        if (current == tujuan) {
            pathCount++;
            cout << "  Jalur " << pathCount << ": ";
            int totalJarak = 0;
            for (size_t i = 0; i < currentPath.size(); i++) {
                cout << currentPath[i];
                if (i < currentPath.size() - 1) {

                    for (const Edge& e : adjList[currentPath[i]]) {
                        if (e.tujuan == currentPath[i + 1]) {
                            totalJarak += e.jarak;
                            cout << " --(" << e.jarak << "km)--> ";
                            break;
                        }
                    }
                }
            }
            cout << " | Total Jarak: " << totalJarak << " km\n";
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

    int getBFSDistance(const string& asal, const string& tujuan) {
        vector<string> path = BFS(asal, tujuan);
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

    bool hasCity(const string& city) const {
        return adjList.find(city) != adjList.end();
    }

    vector<string> getCities() const {
        vector<string> cities;
        for (const auto& pair : adjList) {
            cities.push_back(pair.first);
        }
        return cities;
    }
};

} 