#pragma once
#include <string>

// ============================================================
// UAS: struct (1) - Struct untuk data layanan pengiriman
// Menentukan jenis layanan dan tarif per kg
// ============================================================
struct Layanan {
    int id;
    std::string nama;           // "Reguler", "Express", "Same Day"
    double tarif_per_kg;        // Tarif dasar per kilogram

    // UAS: default argument (6)
    Layanan(int id = 0,
            const std::string& nama = "",
            double tarif_per_kg = 0.0)
        : id(id), nama(nama), tarif_per_kg(tarif_per_kg) {}
};