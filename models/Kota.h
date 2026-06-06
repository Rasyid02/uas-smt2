#pragma once
#include <string>

// ============================================================
// UAS: struct (1) - Struct untuk data kota dan jarak
// Digunakan dalam Graph untuk representasi jalur antar kota
// BFS dan DFS digunakan untuk mencari rute pengiriman
// ============================================================
struct Kota {
    std::string asal;           // Nama kota asal
    std::string tujuan;         // Nama kota tujuan
    int jarak;                  // Jarak dalam kilometer

    // UAS: default argument (6)
    Kota(const std::string& asal = "",
         const std::string& tujuan = "",
         int jarak = 0)
        : asal(asal), tujuan(tujuan), jarak(jarak) {}
};