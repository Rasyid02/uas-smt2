#pragma once
#include <string>

// ============================================================
// UAS: struct (1) - Struct untuk data tracking pengiriman
// Setiap perubahan status akan dicatat sebagai tracking record
// Stack digunakan untuk undo status tracking
// ============================================================
struct Tracking {
    int id;
    int id_paket;               // Foreign key ke Paket
    std::string lokasi;         // Lokasi saat ini
    std::string status;         // Status tracking
    std::string timestamp;      // Waktu update
    std::string keterangan;     // Keterangan tambahan

    // UAS: default argument (6) - Konstruktor dengan nilai default
    Tracking(int id = 0,
             int id_paket = 0,
             const std::string& lokasi = "",
             const std::string& status = "",
             const std::string& timestamp = "",
             const std::string& keterangan = "")
        : id(id), id_paket(id_paket), lokasi(lokasi),
          status(status), timestamp(timestamp), keterangan(keterangan) {}
};