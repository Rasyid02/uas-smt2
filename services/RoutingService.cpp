#include "RoutingService.h"
#include "../storage/CSVManager.h"
#include <iostream>
#include <functional>
using namespace std;

namespace SwiftExpedition {

RoutingService::RoutingService() {
}

void RoutingService::loadKota(const string& filename) {
    try {
        vector<string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            vector<string> t = CSVManager::split(lines[i], ';');
            if (t.size() >= 3) {

                Kota k(t[0], t[1], stoi(t[2]));
                edgesList.push_back(k);

                graph.addEdge(k.asal, k.tujuan, k.jarak);
            }
        }
        cout << "  [LOAD] " << edgesList.size()
                  << " jalur antar kota dimuat dari " << filename << ".\n";
    }
    catch (const exception& e) {
        cout << "  [ERROR] Gagal memuat kota: " << e.what() << "\n";
    }
}

void RoutingService::addRoute(const string& asal, const string& tujuan, int jarak) {
    graph.addEdge(asal, tujuan, jarak);
    edgesList.push_back(Kota(asal, tujuan, jarak));
    cout << "  [ROUTE] " << asal << " <--> " << tujuan << " (" << jarak << " km)\n";
}

void RoutingService::bfsSearch(const string& asal, const string& tujuan) {
    cout << "\n  ===== BFS: Mencari Transit Terdekat =====\n";
    cout << "  Dari: " << asal << " -> Tujuan: " << tujuan << "\n";

    if (!graph.hasCity(asal)) {
        cout << "  [ERROR] Kota asal '" << asal << "' tidak ditemukan.\n";
        return;
    }
    if (!graph.hasCity(tujuan)) {
        cout << "  [ERROR] Kota tujuan '" << tujuan << "' tidak ditemukan.\n";
        return;
    }

    vector<string> path = graph.BFS(asal, tujuan);

    if (path.empty()) {
        cout << "  Tidak ada jalur dari " << asal << " ke " << tujuan << ".\n";
        return;
    }

    cout << "  Jalur terpendek (minimal transit):\n  ";
    int totalJarak = 0;
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }
    totalJarak = graph.getBFSDistance(asal, tujuan);
    cout << "\n  Total jarak: " << totalJarak << " km\n";
    cout << "  Jumlah transit: " << (path.size() - 2 > 0 ? path.size() - 2 : 0) << "\n";
}

void RoutingService::dfsSearch(const string& start) {
    if (!graph.hasCity(start)) {
        cout << "  [ERROR] Kota '" << start << "' tidak ditemukan.\n";
        return;
    }

    graph.DFS(start);
}

void RoutingService::findAllRoutes(const string& asal, const string& tujuan) {
    if (!graph.hasCity(asal) || !graph.hasCity(tujuan)) {
        cout << "  [ERROR] Kota tidak ditemukan dalam graph.\n";
        return;
    }

    graph.findAllPaths(asal, tujuan);
}

void RoutingService::displayEdges() const {
    cout << "\n  ===== DAFTAR JALUR (KOTA.CSV) =====\n";
    if (edgesList.empty()) {
        cout << "  (Tidak ada jalur)\n";
        return;
    }
    for (const Kota& k : edgesList) {
        cout << "  " << k.asal << " -> " << k.tujuan
                  << " (" << k.jarak << " km)\n";
    }
}

void RoutingService::displayGraph() const {
    graph.display();
}

void RoutingService::displayCities() const {
    vector<string> cities = graph.getCities();
    cout << "\n  ===== DAFTAR KOTA =====\n";
    if (cities.empty()) {
        cout << "  (Tidak ada kota)\n";
        return;
    }
    for (size_t i = 0; i < cities.size(); i++) {
        cout << "  " << (i + 1) << ". " << cities[i] << "\n";
    }
}

Graph& RoutingService::getGraph() {
    return graph;
}

int RoutingService::getCityCount() const {
    return graph.getCities().size();
}

vector<string> RoutingService::getAllCities() const {
    return graph.getCities();
}

} 