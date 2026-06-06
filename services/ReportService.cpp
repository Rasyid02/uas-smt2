#include "ReportService.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

namespace SwiftExpedition {

ReportService::ReportService() {}

// UAS: lambda expression (16) - Digunakan untuk filtering dan display
// UAS: sort (12) - Sorting data laporan
// UAS: count (14) - Menghitung statistik
// UAS: callback function (5) - Callback untuk iterasi paket

void ReportService::displayLaporanPaket(std::vector<Paket>& pakets) {
    std::cout << "\n  ============================================\n";
    std::cout << "  === LAPORAN SEMUA PAKET ===\n";
    std::cout << "  ============================================\n";
    
    if (pakets.empty()) {
        std::cout << "  [INFO] Belum ada data paket.\n";
        return;
    }

    // UAS: sort (12) - Urutkan berdasarkan ID
    std::sort(pakets.begin(), pakets.end(), [](const Paket& a, const Paket& b) {
        return a.id < b.id;
    });

    std::cout << "  " << std::setw(5) << "ID" 
              << std::setw(15) << "Resi"
              << std::setw(18) << "Penerima"
              << std::setw(15) << "Kota Tujuan"
              << std::setw(8) << "Berat"
              << std::setw(12) << "Biaya"
              << std::setw(18) << "Status"
              << "\n";
    std::cout << "  " << std::string(91, '-') << "\n";

    // UAS: lambda expression (16) - Display setiap paket
    auto displayRow = [](const Paket& p) {
        std::cout << "  " << std::setw(5) << p.id 
                  << std::setw(15) << p.resi
                  << std::setw(18) << p.nama_penerima
                  << std::setw(15) << p.kota_tujuan
                  << std::setw(8) << p.berat
                  << std::setw(12) << p.biaya
                  << std::setw(18) << p.status
                  << "\n";
    };

    // UAS: callback function (5) - for_each dengan lambda callback
    std::for_each(pakets.begin(), pakets.end(), displayRow);

    std::cout << "\n  Total paket: " << pakets.size() << "\n";
}

void ReportService::displayLaporanByStatus(std::vector<Paket>& pakets) {
    std::cout << "\n  ============================================\n";
    std::cout << "  === LAPORAN PAKET BERDASARKAN STATUS ===\n";
    std::cout << "  ============================================\n";

    if (pakets.empty()) {
        std::cout << "  [INFO] Belum ada data paket.\n";
        return;
    }

    // UAS: count (14) - Hitung per status menggunakan lambda (16)
    auto countByStatus = [&](const std::string& status) -> int {
        return std::count_if(pakets.begin(), pakets.end(),
            [&status](const Paket& p) {
                return p.status == status;
            });
    };

    std::vector<std::string> statuses = {"Menunggu", "Dalam Perjalanan", "Terkirim"};
    for (const auto& s : statuses) {
        int count = countByStatus(s);
        std::cout << "\n  --- Status: " << s << " (" << count << " paket) ---\n";
        for (const auto& p : pakets) {
            if (p.status == s) {
                std::cout << "    ID: " << p.id << " | Resi: " << p.resi 
                          << " | " << p.nama_penerima << " | " << p.kota_tujuan 
                          << " | Rp " << p.biaya << "\n";
            }
        }
    }
}

void ReportService::displayStatistik(std::vector<Paket>& pakets) {
    std::cout << "\n  ============================================\n";
    std::cout << "  === STATISTIK PENGIRIMAN ===\n";
    std::cout << "  ============================================\n";

    if (pakets.empty()) {
        std::cout << "  [INFO] Belum ada data paket.\n";
        return;
    }

    // UAS: count (14) + lambda (16)
    int totalPaket = pakets.size();
    int menunggu = std::count_if(pakets.begin(), pakets.end(),
        [](const Paket& p) { return p.status == "Menunggu"; });
    int dalamPerjalanan = std::count_if(pakets.begin(), pakets.end(),
        [](const Paket& p) { return p.status == "Dalam Perjalanan"; });
    int terkirim = std::count_if(pakets.begin(), pakets.end(),
        [](const Paket& p) { return p.status == "Terkirim"; });

    // UAS: sort (12) + lambda (16) - Hitung rata-rata
    double totalBiaya = 0.0;
    double totalBerat = 0.0;
    double maxBiaya = 0.0;
    std::string paketTermahal;

    for (const auto& p : pakets) {
        totalBiaya += p.biaya;
        totalBerat += p.berat;
        if (p.biaya > maxBiaya) {
            maxBiaya = p.biaya;
            paketTermahal = p.resi;
        }
    }

    double avgBiaya = totalPaket > 0 ? totalBiaya / totalPaket : 0;
    double avgBerat = totalPaket > 0 ? totalBerat / totalPaket : 0;

    std::cout << "\n  Total Paket          : " << totalPaket;
    std::cout << "\n  Menunggu             : " << menunggu;
    std::cout << "\n  Dalam Perjalanan     : " << dalamPerjalanan;
    std::cout << "\n  Terkirim             : " << terkirim;
    std::cout << "\n  ----------------------------------------";
    std::cout << "\n  Total Biaya          : Rp " << std::fixed << std::setprecision(0) << totalBiaya;
    std::cout << "\n  Rata-rata Biaya      : Rp " << avgBiaya;
    std::cout << "\n  Rata-rata Berat      : " << avgBerat << " kg";
    std::cout << "\n  Paket Termahal       : " << paketTermahal << " (Rp " << maxBiaya << ")";
    std::cout << "\n  ============================================\n";
}

void ReportService::displayCEOReport(
    std::vector<Paket>& pakets,
    int totalKurir,
    int totalTracking)
{
    std::cout << "\n  ============================================\n";
    std::cout << "  === LAPORAN CEO - OVERVIEW PERUSAHAAN ===\n";
    std::cout << "  ============================================\n";

    // Panggil displayStatistik untuk statistik dasar
    displayStatistik(pakets);

    std::cout << "\n  ===== INFORMASI TAMBAHAN CEO =====\n";
    std::cout << "  Total Kurir Aktif    : " << totalKurir << "\n";
    std::cout << "  Total Tracking Entry : " << totalTracking << "\n";
    std::cout << "  ============================================\n";
}

} // namespace SwiftExpedition