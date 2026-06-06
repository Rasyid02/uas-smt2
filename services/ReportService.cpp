#include "ReportService.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;

namespace SwiftExpedition {

ReportService::ReportService() {}

void ReportService::displayLaporanPaket(vector<Paket>& pakets) {
    cout << "\n  ============================================\n";
    cout << "  === LAPORAN SEMUA PAKET ===\n";
    cout << "  ============================================\n";

    if (pakets.empty()) {
        cout << "  [INFO] Belum ada data paket.\n";
        return;
    }

    sort(pakets.begin(), pakets.end(), [](const Paket& a, const Paket& b) {
        return a.id < b.id;
    });

    cout << "  " << setw(5) << "ID" 
              << setw(15) << "Resi"
              << setw(18) << "Penerima"
              << setw(15) << "Kota Tujuan"
              << setw(8) << "Berat"
              << setw(12) << "Biaya"
              << setw(18) << "Status"
              << "\n";
    cout << "  " << string(91, '-') << "\n";

    auto displayRow = [](const Paket& p) {
        cout << "  " << setw(5) << p.id 
                  << setw(15) << p.resi
                  << setw(18) << p.nama_penerima
                  << setw(15) << p.kota_tujuan
                  << setw(8) << p.berat
                  << setw(12) << p.biaya
                  << setw(18) << p.status
                  << "\n";
    };

    for_each(pakets.begin(), pakets.end(), displayRow);

    cout << "\n  Total paket: " << pakets.size() << "\n";
}

void ReportService::displayLaporanByStatus(vector<Paket>& pakets) {
    cout << "\n  ============================================\n";
    cout << "  === LAPORAN PAKET BERDASARKAN STATUS ===\n";
    cout << "  ============================================\n";

    if (pakets.empty()) {
        cout << "  [INFO] Belum ada data paket.\n";
        return;
    }

    auto countByStatus = [&](const string& status) -> int {
        return count_if(pakets.begin(), pakets.end(),
            [&status](const Paket& p) {
                return p.status == status;
            });
    };

    vector<string> statuses = {"Menunggu", "Dalam Perjalanan", "Terkirim"};
    for (const auto& s : statuses) {
        int count = countByStatus(s);
        cout << "\n  --- Status: " << s << " (" << count << " paket) ---\n";
        for (const auto& p : pakets) {
            if (p.status == s) {
                cout << "    ID: " << p.id << " | Resi: " << p.resi 
                          << " | " << p.nama_penerima << " | " << p.kota_tujuan 
                          << " | Rp " << p.biaya << "\n";
            }
        }
    }
}

void ReportService::displayStatistik(vector<Paket>& pakets) {
    cout << "\n  ============================================\n";
    cout << "  === STATISTIK PENGIRIMAN ===\n";
    cout << "  ============================================\n";

    if (pakets.empty()) {
        cout << "  [INFO] Belum ada data paket.\n";
        return;
    }

    int totalPaket = pakets.size();
    int menunggu = count_if(pakets.begin(), pakets.end(),
        [](const Paket& p) { return p.status == "Menunggu"; });
    int dalamPerjalanan = count_if(pakets.begin(), pakets.end(),
        [](const Paket& p) { return p.status == "Dalam Perjalanan"; });
    int terkirim = count_if(pakets.begin(), pakets.end(),
        [](const Paket& p) { return p.status == "Terkirim"; });

    double totalBiaya = 0.0;
    double totalBerat = 0.0;
    double maxBiaya = 0.0;
    string paketTermahal;

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

    cout << "\n  Total Paket          : " << totalPaket;
    cout << "\n  Menunggu             : " << menunggu;
    cout << "\n  Dalam Perjalanan     : " << dalamPerjalanan;
    cout << "\n  Terkirim             : " << terkirim;
    cout << "\n  ----------------------------------------";
    cout << "\n  Total Biaya          : Rp " << fixed << setprecision(0) << totalBiaya;
    cout << "\n  Rata-rata Biaya      : Rp " << avgBiaya;
    cout << "\n  Rata-rata Berat      : " << avgBerat << " kg";
    cout << "\n  Paket Termahal       : " << paketTermahal << " (Rp " << maxBiaya << ")";
    cout << "\n  ============================================\n";
}

void ReportService::displayCEOReport(
    vector<Paket>& pakets,
    int totalKurir,
    int totalTracking)
{
    cout << "\n  ============================================\n";
    cout << "  === LAPORAN CEO - OVERVIEW PERUSAHAAN ===\n";
    cout << "  ============================================\n";

    displayStatistik(pakets);

    cout << "\n  ===== INFORMASI TAMBAHAN CEO =====\n";
    cout << "  Total Kurir Aktif    : " << totalKurir << "\n";
    cout << "  Total Tracking Entry : " << totalTracking << "\n";
    cout << "  ============================================\n";
}

} 