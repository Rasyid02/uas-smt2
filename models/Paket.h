#pragma once
#include <string>

// ============================================================
// UAS: struct (1) - Struct untuk data paket pengiriman
// Menyimpan seluruh informasi paket yang dikirim
// ============================================================
struct Paket {
    int id;
    std::string resi;           // Nomor resi unik (digunakan AVL Tree untuk pencarian)
    std::string nama_penerima;
    std::string alamat_tujuan;
    std::string kota_asal;
    std::string kota_tujuan;
    double berat;               // Berat dalam kg
    double biaya;               // Biaya pengiriman
    std::string status;         // Status: "menunggu", "diproses", "dalam_perjalanan", "sampai_tujuan", "terkirim"
    int id_layanan;             // Foreign key ke Layanan
    int id_klasifikasi;         // Foreign key ke KlasifikasiBerat
    int id_kurir;               // Foreign key ke Kurir (0 = belum diassign)

    // UAS: default argument (6) - Konstruktor dengan nilai default
    Paket(int id = 0,
          const std::string& resi = "",
          const std::string& nama_penerima = "",
          const std::string& alamat_tujuan = "",
          const std::string& kota_asal = "",
          const std::string& kota_tujuan = "",
          double berat = 0.0,
          double biaya = 0.0,
          const std::string& status = "menunggu",
          int id_layanan = 0,
          int id_klasifikasi = 0,
          int id_kurir = 0)
        : id(id), resi(resi), nama_penerima(nama_penerima),
          alamat_tujuan(alamat_tujuan), kota_asal(kota_asal),
          kota_tujuan(kota_tujuan), berat(berat), biaya(biaya),
          status(status), id_layanan(id_layanan),
          id_klasifikasi(id_klasifikasi), id_kurir(id_kurir) {}
};