#pragma once
#include <string>

// ============================================================
// UAS: struct (1) - Struct untuk data kurir
// Circular Linked List digunakan untuk rotasi kurir
// saat mengassign paket secara adil dan bergiliran
// ============================================================
struct Kurir {
    int id;
    std::string nama;
    std::string status;         // "Tersedia" atau "Sibuk"
    int total_paket;            // Jumlah paket yang sedang ditangani

    // UAS: default argument (6)
    Kurir(int id = 0,
          const std::string& nama = "",
          const std::string& status = "Tersedia",
          int total_paket = 0)
        : id(id), nama(nama), status(status), total_paket(total_paket) {}
};