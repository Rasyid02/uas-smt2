#include "ManagerMenu.h"
#include "../services/PaketService.h"
#include "../services/ReportService.h"
#include "../services/TrackingService.h"
#include <iostream>
#include <limits>
using namespace std;

namespace SwiftExpedition {

bool ManagerMenu::show(User* user, PaketService& paket, ReportService& report, TrackingService& tracking) {
    int pilihan;
    do {
        cout << "\n  ============================================\n";
        cout << "  === MENU MANAGER ===\n";
        cout << "  User: " << user->nama << "\n";
        cout << "  ============================================\n";
        cout << "  1. Laporan Semua Paket\n";
        cout << "  2. Laporan Paket by Status\n";
        cout << "  3. Sorting Paket Berdasarkan Biaya\n";
        cout << "  4. Sorting Paket Berdasarkan Berat\n";
        cout << "  5. Statistik Pengiriman\n";
        cout << "  6. Lihat Riwayat Tracking\n";
        cout << "  7. Lihat Semua Paket\n";
        cout << "  0. Logout\n";
        cout << "  99. Exit Program\n";
        cout << "  ----------------------------------------\n";
        cout << "  Pilihan: ";
        cin >> pilihan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
            cout << "\n  1. Ascending (termurah)\n  2. Descending (termahal)\n  Pilih: ";
            int s; cin >> s;
            auto sorted = paket.sortByBiaya(s == 1);
            cout << "\n  ===== PAKET DIURUTKAN BERDASARKAN BIAYA =====\n";
            for (const auto& p : sorted) {
                cout << "  " << p.id << " | " << p.resi << " | " << p.nama_penerima
                          << " | Rp " << p.biaya << " | " << p.status << "\n";
            }
            break;
        }
        case 4: {
            cout << "\n  1. Ascending (teringan)\n  2. Descending (terberat)\n  Pilih: ";
            int s; cin >> s;
            auto sorted = paket.sortByBerat(s == 1);
            cout << "\n  ===== PAKET DIURUTKAN BERDASARKAN BERAT =====\n";
            for (const auto& p : sorted) {
                cout << "  " << p.id << " | " << p.resi << " | " << p.nama_penerima
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
        case 0: cout << "  Logout...\n"; return true;
        case 99: cout << "\n  Keluar dari program...\n"; return false;
        default: cout << "  Pilihan tidak valid!\n"; break;
        }
    } while (true);
}

} 