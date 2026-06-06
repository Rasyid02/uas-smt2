#include "ManagerMenu.h"
#include "../services/PaketService.h"
#include "../services/ReportService.h"
#include "../services/TrackingService.h"
#include <iostream>
#include <limits>

namespace SwiftExpedition {

bool ManagerMenu::show(User* user, PaketService& paket, ReportService& report, TrackingService& tracking) {
    int pilihan;
    do {
        std::cout << "\n  ============================================\n";
        std::cout << "  === MENU MANAGER ===\n";
        std::cout << "  User: " << user->nama << "\n";
        std::cout << "  ============================================\n";
        std::cout << "  1. Laporan Semua Paket\n";
        std::cout << "  2. Laporan Paket by Status\n";
        std::cout << "  3. Sorting Paket Berdasarkan Biaya\n";
        std::cout << "  4. Sorting Paket Berdasarkan Berat\n";
        std::cout << "  5. Statistik Pengiriman\n";
        std::cout << "  6. Lihat Riwayat Tracking\n";
        std::cout << "  7. Lihat Semua Paket\n";
        std::cout << "  0. Logout\n";
        std::cout << "  99. Exit Program\n";
        std::cout << "  ----------------------------------------\n";
        std::cout << "  Pilihan: ";
        std::cin >> pilihan;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (pilihan) {
        case 1: {
            auto pakets = paket.getAllPaket();
            report.displayLaporanPaket(pakets);
            break;
        }
        case 2: {
            auto pakets = paket.getAllPaket();
            report.displayLaporanByStatus(pakets);
            break;
        }
        case 3: {
            std::cout << "\n  1. Ascending (termurah)\n  2. Descending (termahal)\n  Pilih: ";
            int s; std::cin >> s;
            auto sorted = paket.sortByBiaya(s == 1);
            std::cout << "\n  ===== PAKET DIURUTKAN BERDASARKAN BIAYA =====\n";
            for (const auto& p : sorted) {
                std::cout << "  " << p.id << " | " << p.resi << " | " << p.nama_penerima
                          << " | Rp " << p.biaya << " | " << p.status << "\n";
            }
            break;
        }
        case 4: {
            std::cout << "\n  1. Ascending (teringan)\n  2. Descending (terberat)\n  Pilih: ";
            int s; std::cin >> s;
            auto sorted = paket.sortByBerat(s == 1);
            std::cout << "\n  ===== PAKET DIURUTKAN BERDASARKAN BERAT =====\n";
            for (const auto& p : sorted) {
                std::cout << "  " << p.id << " | " << p.resi << " | " << p.nama_penerima
                          << " | " << p.berat << " kg | " << p.status << "\n";
            }
            break;
        }
        case 5: {
            auto pakets = paket.getAllPaket();
            report.displayStatistik(pakets);
            break;
        }
        case 6: {
            tracking.displayAllHistory();
            break;
        }
        case 7: paket.displayAllPaket(); break;
        case 0: std::cout << "  Logout...\n"; return true;
        case 99: std::cout << "\n  Keluar dari program...\n"; return false;
        default: std::cout << "  Pilihan tidak valid!\n"; break;
        }
    } while (true);
}

} // namespace SwiftExpedition