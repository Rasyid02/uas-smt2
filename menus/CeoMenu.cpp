#include "CeoMenu.h"
#include "../services/PaketService.h"
#include "../services/ReportService.h"
#include "../services/TrackingService.h"
#include "../services/RoutingService.h"
#include <iostream>
#include <limits>

namespace SwiftExpedition {

bool CeoMenu::show(User* user, PaketService& paket, ReportService& report,
                    TrackingService& tracking, RoutingService& routing) {
    int pilihan;
    do {
        std::cout << "\n  ============================================\n";
        std::cout << "  === MENU CEO ===\n";
        std::cout << "  User: " << user->nama << " (CEO)\n";
        std::cout << "  ============================================\n";
        std::cout << "  1. Laporan Keseluruhan (CEO Report)\n";
        std::cout << "  2. Statistik Pengiriman\n";
        std::cout << "  3. Lihat Semua Paket\n";
        std::cout << "  4. Lihat Riwayat Tracking\n";
        std::cout << "  5. Lihat Graph Rute Kota\n";
        std::cout << "  6. BFS - Cari Jalur Terpendek Antar Kota\n";
        std::cout << "  7. DFS - Tampilkan Semua Rute Dari Kota\n";
        std::cout << "  8. Cari Semua Jalur Antar Kota\n";
        std::cout << "  9. Lihat Daftar Kota\n";
        std::cout << "  10. Simpan Data\n";
        std::cout << "  0. Logout\n";
        std::cout << "  99. Exit Program\n";
        std::cout << "  ----------------------------------------\n";
        std::cout << "  Pilihan: ";
        std::cin >> pilihan;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (pilihan) {
        case 1: {
            auto pakets = paket.getAllPaket();
            int totalKurir = 1; // simplified
            int totalTracking = 1;
            report.displayCEOReport(pakets, totalKurir, totalTracking);
            break;
        }
        case 2: {
            auto pakets = paket.getAllPaket();
            report.displayStatistik(pakets);
            break;
        }
        case 3: paket.displayAllPaket(); break;
        case 4: tracking.displayAllHistory(); break;
        case 5: {
            routing.displayEdges();
            break;
        }
        case 6: {
            std::string asal, tujuan;
            std::cout << "\n  Kota Asal  : "; std::getline(std::cin, asal);
            std::cout << "  Kota Tujuan: "; std::getline(std::cin, tujuan);
            routing.bfsSearch(asal, tujuan);
            break;
        }
        case 7: {
            std::string start;
            std::cout << "\n  Kota Awal: "; std::getline(std::cin, start);
            routing.dfsSearch(start);
            break;
        }
        case 8: {
            std::string asal, tujuan;
            std::cout << "\n  Kota Asal  : "; std::getline(std::cin, asal);
            std::cout << "  Kota Tujuan: "; std::getline(std::cin, tujuan);
            routing.findAllRoutes(asal, tujuan);
            break;
        }
        case 9: {
            routing.displayCities();
            break;
        }
        case 10: {
            paket.savePaket("paket.csv");
            tracking.saveTracking("tracking.csv");
            break;
        }
        case 0: std::cout << "  Logout...\n"; return true;
        case 99: std::cout << "\n  Keluar dari program...\n"; return false;
        default: std::cout << "  Pilihan tidak valid!\n"; break;
        }
    } while (true);
}

} // namespace SwiftExpedition