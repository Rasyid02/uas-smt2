#include "KurirMenu.h"
#include "../services/PaketService.h"
#include "../services/TrackingService.h"
#include <iostream>
#include <limits>
using namespace std;

namespace SwiftExpedition {

bool KurirMenu::show(User* user, PaketService& paket, TrackingService& tracking) {
    int pilihan;
    do {
        cout << "\n  ============================================\n";
        cout << "  === MENU KURIR ===\n";
        cout << "  User: " << user->nama << "\n";
        cout << "  ============================================\n";
        cout << "  1. Ambil Paket dari Queue\n";
        cout << "  2. Update Status Tracking\n";
        cout << "  3. Undo Tracking (Stack)\n";
        cout << "  4. Lihat Riwayat Tracking\n";
        cout << "  5. Lihat Antrean Paket\n";
        cout << "  6. Lihat Semua Paket\n";
        cout << "  0. Logout\n";
        cout << "  99. Exit Program\n";
        cout << "  ----------------------------------------\n";
        cout << "  Pilihan: ";
        cin >> pilihan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
        case 1: { 
            Paket p;
            if (paket.dequeuePaket(p)) {
                cout << "\n  [BERHASIL] Mengambil paket " << p.resi << "\n";
                cout << "  Penerima: " << p.nama_penerima << "\n";
                cout << "  Tujuan  : " << p.kota_asal << " -> " << p.kota_tujuan << "\n";

                Kurir* k = paket.getNextKurir();
                if (k) {
                    paket.assignPaketToKurir(p.id, k->id);
                    tracking.updateStatus(p.id, "Dalam Perjalanan", p.kota_asal);
                } else {
                    tracking.updateStatus(p.id, "Diproses", "Pusat Sortir");
                }
            } else {
                cout << "\n  [KOSONG] Tidak ada paket dalam antrean.\n";
            }
            break;
        }
        case 2: { 
            int id;
            string status, lokasi;
            cout << "\n  ID Paket  : "; cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Status Baru: "; getline(cin, status);
            cout << "  Lokasi     : "; getline(cin, lokasi);
            tracking.updateStatus(id, status, lokasi);

            Paket* p = paket.findPaketById(id);
            if (p) { p->status = status; }
            break;
        }
        case 3: { 
            int id;
            cout << "\n  ID Paket untuk undo: "; cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (tracking.undoLastTracking(id)) {
                Tracking latest = tracking.getLatestStatus(id);
                Paket* p = paket.findPaketById(id);
                if (p && !latest.status.empty()) p->status = latest.status;
                cout << "  [UNDO] Kembali ke status: " << latest.status << "\n";
            } else {
                cout << "  [GAGAL] Tidak ada tracking yang bisa di-undo.\n";
            }
            break;
        }
        case 4: { 
            cout << "\n  1. Lihat per Paket (masukkan ID)\n  2. Lihat semua\n  Pilih: ";
            int sub; cin >> sub;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (sub == 1) {
                int id; cout << "  ID Paket: "; cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                tracking.displayHistory(id);
            } else {
                tracking.displayAllHistory();
            }
            break;
        }
        case 5: paket.displayQueue(); break;
        case 6: paket.displayAllPaket(); break;
        case 0: cout << "  Logout...\n"; return true;
        case 99: cout << "\n  Keluar dari program...\n"; return false;
        default: cout << "  Pilihan tidak valid!\n"; break;
        }
    } while (true);
}

} 