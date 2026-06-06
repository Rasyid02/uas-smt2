#pragma once
#include <string>

// ============================================================
// UAS: struct (1) - Struct untuk klasifikasi berat paket
// Menentukan kategori berat dan biaya tambahan
// ============================================================
struct KlasifikasiBerat {
    int id;
    std::string nama;           // "Ringan" (0-5kg), "Sedang" (5-20kg), "Berat" (>20kg)
    double biaya_tambahan;      // Biaya tambahan berdasarkan klasifikasi

    // UAS: default argument (6)
    KlasifikasiBerat(int id = 0,
                     const std::string& nama = "",
                     double biaya_tambahan = 0.0)
        : id(id), nama(nama), biaya_tambahan(biaya_tambahan) {}
};