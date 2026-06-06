#pragma once
#include <string>
#include <vector>
#include "../datastructures/Graph.h"
#include "../models/Kota.h"

// ============================================================
// UAS: RoutingService - Navigasi jalur pengiriman
// UAS: Graph (6), BFS (7), DFS (8)
// ============================================================

namespace SwiftExpedition {

class RoutingService {
private:
    Graph graph;                       // UAS: Graph (6)
    std::vector<Kota> edgesList;       // Daftar edge dari CSV

public:
    RoutingService();

    // ========== LOAD DATA ==========
    void loadKota(const std::string& filename);
    // Membangun graph otomatis berdasarkan edges yang diload

    // ========== ROUTING ==========
    void addRoute(const std::string& asal, const std::string& tujuan, int jarak);

    // UAS: BFS (7) - Cari jalur terpendek
    void bfsSearch(const std::string& asal, const std::string& tujuan);

    // UAS: DFS (8) - Tampilkan semua jalur
    void dfsSearch(const std::string& start);
    void findAllRoutes(const std::string& asal, const std::string& tujuan);

    // ========== DISPLAY ==========
    void displayEdges() const;
    void displayGraph() const;
    void displayCities() const;

    // ========== GETTERS ==========
    Graph& getGraph();
    int getCityCount() const;
    std::vector<std::string> getAllCities() const;
};

} // namespace SwiftExpedition