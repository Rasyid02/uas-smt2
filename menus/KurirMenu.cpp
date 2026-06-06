#include "KurirMenu.h"
#include "../services/PaketService.h"
#include "../services/TrackingService.h"
#include <iostream>
#include <limits>

namespace SwiftExpedition {

bool KurirMenu::show(User* user, PaketService& paket, TrackingService& tracking) {
    int pilihan;
    do {
        std::cout << "\n  ============================================\n";
        std::cout << "  === MENU KURIR ===\n";
        std::cout << "  User: " << user->nama << "\n";
        std::cout << "  ============================================\n";
        std::cout << "  1. Ambil Paket dari Queue\n";
        std::cout << "  2. Update Status Tracking\n";
        std::cout << "  3. Undo Tracking (Stack)\n";
        std::cout << "  4. Lihat Riwayat Tracking\n";
        std::cout << "  5. Lihat Antrean Paket\n";
        std::cout << "  6. Lihat Semua Paket\n";
        std::cout << "  0. Logout\n";
        std::cout << "  99. Exit Program\n";
        std::cout << "  ----------------------------------------\n";
        std::cout << "  Pilihan: ";
        std::cin >> pilihan;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (pilihan) {
        case 1: { // Ambil paket dari queue
            Paket p;
            if (paket.dequeuePaket(p)) {
                std::cout << "\n  [BERHASIL] Mengambil paket " << p.resi << "\n";
                std::cout << "  Penerima: " << p.nama_penerima << "\n";
                std::cout << "  Tujuan  : " << p.kota_asal << " -> " << p.kota_tujuan << "\n";
                // Assign ke kurir (rotasi circular linked list)
                Kurir* k = paket.getNextKurir();
                if (k) {
                    paket.assignPaketToKurir(p.id, k->id);
                    tracking.updateStatus(p.id, "Dalam Perjalanan", p.kota_asal);
                } else {
                    tracking.updateStatus(p.id, "Diproses", "Pusat Sortir");
                }
            } else {
                std::cout << "\n  [KOSONG] Tidak ada paket dalam antrean.\n";
            }
            break;
        }
        case 2: { // Update tracking
            int id;
            std::string status, lokasi;
            std::cout << "\n  ID Paket  : "; std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Status Baru: "; std::getline(std::cin, status);
            std::cout << "  Lokasi     : "; std::getline(std::cin, lokasi);
            tracking.updateStatus(id, status, lokasi);
            // Update paket juga
            Paket* p = paket.findPaketById(id);
            if (p) { p->status = status; }
            break;
        }
        case 3: { // Undo tracking (Stack)
            int id;
            std::cout << "\n  ID Paket untuk undo: "; std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (tracking.undoLastTracking(id)) {
                Tracking latest = tracking.getLatestStatus(id);
                Paket* p = paket.findPaketById(id);
                if (p && !latest.status.empty()) p->status = latest.status;
                std::cout << "  [UNDO] Kembali ke status: " << latest.status << "\n";
            } else {
                std::cout << "  [GAGAL] Tidak ada tracking yang bisa di-undo.\n";
            }
            break;
        }
        case 4: { // Riwayat tracking
            std::cout << "\n  1. Lihat per Paket (masukkan ID)\n  2. Lihat semua\n  Pilih: ";
            int sub; std::cin >> sub;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (sub == 1) {
                int id; std::cout << "  ID Paket: "; std::cin >> id;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                tracking.displayHistory(id);
            } else {
                tracking.displayAllHistory();
            }
            break;
        }
        case 5: paket.displayQueue(); break;
        case 6: paket.displayAllPaket(); break;
        case 0: std::cout << "  Logout...\n"; return true;
        case 99: std::cout << "\n  Keluar dari program...\n"; return false;
        default: std::cout << "  Pilihan tidak valid!\n"; break;
        }
    } while (true);
}

} // namespace SwiftExpedition