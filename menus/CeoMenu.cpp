#include "CeoMenu.h"
#include "../services/PaketService.h"
#include "../services/ReportService.h"
#include "../services/TrackingService.h"
#include "../services/RoutingService.h"
#include <iostream>
#include <limits>
using namespace std;

namespace SwiftExpedition {

bool CeoMenu::show(User* user, PaketService& paket, ReportService& report,
                    TrackingService& tracking, RoutingService& routing) {
    int pilihan;
    do {
        cout << "\n  ============================================\n";
        cout << "  === MENU CEO ===\n";
        cout << "  User: " << user->nama << " (CEO)\n";
        cout << "  ============================================\n";
        cout << "  1. Laporan Keseluruhan (CEO Report)\n";
        cout << "  2. Statistik Pengiriman\n";
        cout << "  3. Lihat Semua Paket\n";
        cout << "  4. Lihat Riwayat Tracking\n";
        cout << "  5. Lihat Graph Rute Kota\n";
        cout << "  6. BFS - Cari Jalur Terpendek Antar Kota\n";
        cout << "  7. DFS - Tampilkan Semua Rute Dari Kota\n";
        cout << "  8. Cari Semua Jalur Antar Kota\n";
        cout << "  9. Lihat Daftar Kota\n";
        cout << "  10. Simpan Data\n";
        cout << "  0. Logout\n";
        cout << "  99. Exit Program\n";
        cout << "  ----------------------------------------\n";
        cout << "  Pilihan: ";
        cin >> pilihan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
        case 1: {
            auto pakets = paket.getAllPaket();
            int totalKurir = 1; 
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
            string asal, tujuan;
            cout << "\n  Kota Asal  : "; getline(cin, asal);
            cout << "  Kota Tujuan: "; getline(cin, tujuan);
            routing.bfsSearch(asal, tujuan);
            break;
        }
        case 7: {
            string start;
            cout << "\n  Kota Awal: "; getline(cin, start);
            routing.dfsSearch(start);
            break;
        }
        case 8: {
            string asal, tujuan;
            cout << "\n  Kota Asal  : "; getline(cin, asal);
            cout << "  Kota Tujuan: "; getline(cin, tujuan);
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
        case 0: cout << "  Logout...\n"; return true;
        case 99: cout << "\n  Keluar dari program...\n"; return false;
        default: cout << "  Pilihan tidak valid!\n"; break;
        }
    } while (true);
}

} 