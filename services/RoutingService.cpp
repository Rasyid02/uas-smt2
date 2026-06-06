#include "RoutingService.h"
#include "../storage/CSVManager.h"
#include <iostream>
#include <functional>

// ============================================================
// UAS: RoutingService - Implementasi navigasi
// UAS: Graph (6), BFS (7), DFS (8)
// ============================================================

namespace SwiftExpedition {

RoutingService::RoutingService() {
}

// ============================================================
// LOAD DATA: Membaca edge dari CSV
// Format: asal;tujuan;jarak
// ============================================================
void RoutingService::loadKota(const std::string& filename) {
    try {
        std::vector<std::string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            std::vector<std::string> t = CSVManager::split(lines[i], ';');
            if (t.size() >= 3) {
                // UAS: default argument (6) - Kota constructor
                Kota k(t[0], t[1], std::stoi(t[2]));
                edgesList.push_back(k);

                // UAS: Graph (6) - Tambahkan edge ke graph
                graph.addEdge(k.asal, k.tujuan, k.jarak);
            }
        }
        std::cout << "  [LOAD] " << edgesList.size()
                  << " jalur antar kota dimuat dari " << filename << ".\n";
    }
    catch (const std::exception& e) {
        std::cout << "  [ERROR] Gagal memuat kota: " << e.what() << "\n";
    }
}

// ============================================================
// GRAPH - Tambah rute
// ============================================================
void RoutingService::addRoute(const std::string& asal, const std::string& tujuan, int jarak) {
    graph.addEdge(asal, tujuan, jarak);
    edgesList.push_back(Kota(asal, tujuan, jarak));
    std::cout << "  [ROUTE] " << asal << " <--> " << tujuan << " (" << jarak << " km)\n";
}

// ============================================================
// BFS (7) - Mencari jalur transit terdekat
// Menggunakan BFS bawaan Graph
// ============================================================
void RoutingService::bfsSearch(const std::string& asal, const std::string& tujuan) {
    std::cout << "\n  ===== BFS: Mencari Transit Terdekat =====\n";
    std::cout << "  Dari: " << asal << " -> Tujuan: " << tujuan << "\n";

    if (!graph.hasCity(asal)) {
        std::cout << "  [ERROR] Kota asal '" << asal << "' tidak ditemukan.\n";
        return;
    }
    if (!graph.hasCity(tujuan)) {
        std::cout << "  [ERROR] Kota tujuan '" << tujuan << "' tidak ditemukan.\n";
        return;
    }

    // UAS: BFS (7) - Gunakan BFS bawaan Graph
    std::vector<std::string> path = graph.BFS(asal, tujuan);

    if (path.empty()) {
        std::cout << "  Tidak ada jalur dari " << asal << " ke " << tujuan << ".\n";
        return;
    }

    // UAS: iterator (11) - traverse path
    std::cout << "  Jalur terpendek (minimal transit):\n  ";
    int totalJarak = 0;
    for (size_t i = 0; i < path.size(); i++) {
        std::cout << path[i];
        if (i < path.size() - 1) {
            std::cout << " -> ";
        }
    }
    totalJarak = graph.getBFSDistance(asal, tujuan);
    std::cout << "\n  Total jarak: " << totalJarak << " km\n";
    std::cout << "  Jumlah transit: " << (path.size() - 2 > 0 ? path.size() - 2 : 0) << "\n";
}

// ============================================================
// DFS (8) - Traversal dari suatu kota
// Menggunakan DFS bawaan Graph
// ============================================================
void RoutingService::dfsSearch(const std::string& start) {
    if (!graph.hasCity(start)) {
        std::cout << "  [ERROR] Kota '" << start << "' tidak ditemukan.\n";
        return;
    }

    // UAS: DFS (8) - Gunakan DFS bawaan Graph
    graph.DFS(start);
}

// ============================================================
// FIND ALL ROUTES - Semua jalur yang mungkin
// ============================================================
void RoutingService::findAllRoutes(const std::string& asal, const std::string& tujuan) {
    if (!graph.hasCity(asal) || !graph.hasCity(tujuan)) {
        std::cout << "  [ERROR] Kota tidak ditemukan dalam graph.\n";
        return;
    }

    // Gunakan findAllPaths dari Graph
    graph.findAllPaths(asal, tujuan);
}

// ============================================================
// DISPLAY
// ============================================================
void RoutingService::displayEdges() const {
    std::cout << "\n  ===== DAFTAR JALUR (KOTA.CSV) =====\n";
    if (edgesList.empty()) {
        std::cout << "  (Tidak ada jalur)\n";
        return;
    }
    for (const Kota& k : edgesList) {
        std::cout << "  " << k.asal << " -> " << k.tujuan
                  << " (" << k.jarak << " km)\n";
    }
}

void RoutingService::displayGraph() const {
    graph.display();
}

void RoutingService::displayCities() const {
    std::vector<std::string> cities = graph.getCities();
    std::cout << "\n  ===== DAFTAR KOTA =====\n";
    if (cities.empty()) {
        std::cout << "  (Tidak ada kota)\n";
        return;
    }
    for (size_t i = 0; i < cities.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << cities[i] << "\n";
    }
}

// ============================================================
// GETTERS
// ============================================================
Graph& RoutingService::getGraph() {
    return graph;
}

int RoutingService::getCityCount() const {
    return graph.getCities().size();
}

std::vector<std::string> RoutingService::getAllCities() const {
    return graph.getCities();
}

} // namespace SwiftExpedition