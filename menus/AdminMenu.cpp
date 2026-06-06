#include "AdminMenu.h"
#include "../services/PaketService.h"
#include <iostream>
#include <limits>
#include <string>
using namespace std;

namespace SwiftExpedition {

bool AdminMenu::show(User* user, PaketService& paket) {
    int pilihan;
    do {
        cout << "\n  ============================================\n";
        cout << "  === MENU ADMIN ===\n";
        cout << "  User: " << user->nama << "\n";
        cout << "  ============================================\n";
        cout << "  1. Tambah Paket\n";
        cout << "  2. Hapus Paket\n";
        cout << "  3. Edit Paket\n";
        cout << "  4. Cari Paket (by ID)\n";
        cout << "  5. Cari Paket (by Resi)\n";
        cout << "  6. Lihat Semua Paket\n";
        cout << "  7. Lihat Antrean Paket\n";
        cout << "  8. Lihat Daftar Kurir\n";
        cout << "  9. Lihat Daftar Layanan\n";
        cout << "  10. Lihat Klasifikasi Berat\n";
        cout << "  11. Lihat AVL Tree (Resi)\n";
        cout << "  0. Logout\n";
        cout << "  99. Exit Program\n";
        cout << "  ----------------------------------------\n";
        cout << "  Pilihan: ";
        cin >> pilihan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
        case 1: { 
            Paket p;
            p.id = 0; 
            cout << "\n  --- Tambah Paket Baru ---\n";
            cout << "  Resi          : "; getline(cin, p.resi);
            cout << "  Nama Penerima : "; getline(cin, p.nama_penerima);
            cout << "  Alamat Tujuan : "; getline(cin, p.alamat_tujuan);
            cout << "  Kota Asal     : "; getline(cin, p.kota_asal);
            cout << "  Kota Tujuan   : "; getline(cin, p.kota_tujuan);
            cout << "  Berat (kg)    : "; cin >> p.berat;
            cout << "  ID Layanan    : "; cin >> p.id_layanan;
            cout << "  ID Klasifikasi: "; cin >> p.id_klasifikasi;
            cout << "  Biaya         : "; cin >> p.biaya;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            p.status = "Pending";
            p.id_kurir = 0;
            paket.addPaket(p);

            paket.enqueuePaket(p);
            cout << "\n  [SUCCESS] Paket berhasil ditambahkan dan dimasukkan ke antrean!\n";
            break;
        }
        case 2: { 
            int id;
            cout << "\n  Masukkan ID paket yang akan dihapus: ";
            cin >> id;
            if (paket.deletePaket(id)) {
                cout << "  [SUCCESS] Paket berhasil dihapus!\n";
            } else {
                cout << "  [ERROR] Paket tidak ditemukan!\n";
            }
            break;
        }
        case 3: { 
            int id;
            cout << "\n  Masukkan ID paket yang akan diedit: ";
            cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Paket* existing = paket.findPaketById(id);
            if (!existing) {
                cout << "  [ERROR] Paket tidak ditemukan!\n";
                break;
            }

            Paket newData = *existing;
            cout << "  --- Edit Paket (kosongkan untuk tidak mengubah) ---\n";
            string input;

            cout << "  Resi [" << newData.resi << "]: "; getline(cin, input);
            if (!input.empty()) newData.resi = input;

            cout << "  Nama Penerima [" << newData.nama_penerima << "]: "; getline(cin, input);
            if (!input.empty()) newData.nama_penerima = input;

            cout << "  Alamat Tujuan [" << newData.alamat_tujuan << "]: "; getline(cin, input);
            if (!input.empty()) newData.alamat_tujuan = input;

            cout << "  Kota Asal [" << newData.kota_asal << "]: "; getline(cin, input);
            if (!input.empty()) newData.kota_asal = input;

            cout << "  Kota Tujuan [" << newData.kota_tujuan << "]: "; getline(cin, input);
            if (!input.empty()) newData.kota_tujuan = input;

            cout << "  Status [" << newData.status << "]: "; getline(cin, input);
            if (!input.empty()) newData.status = input;

            cout << "  Berat [" << newData.berat << "]: "; getline(cin, input);
            if (!input.empty()) newData.berat = stod(input);

            cout << "  Biaya [" << newData.biaya << "]: "; getline(cin, input);
            if (!input.empty()) newData.biaya = stod(input);

            if (paket.editPaket(id, newData)) {
                cout << "  [SUCCESS] Paket berhasil diupdate!\n";
            } else {
                cout << "  [ERROR] Gagal mengupdate paket!\n";
            }
            break;
        }
        case 4: { 
            int id;
            cout << "\n  Masukkan ID paket: ";
            cin >> id;
            Paket* found = paket.findPaketById(id);
            if (found) {
                cout << "\n  ===== PAKET DITEMUKAN =====\n";
                cout << "  ID       : " << found->id << "\n";
                cout << "  Resi     : " << found->resi << "\n";
                cout << "  Penerima : " << found->nama_penerima << "\n";
                cout << "  Alamat   : " << found->alamat_tujuan << "\n";
                cout << "  Dari     : " << found->kota_asal << " -> " << found->kota_tujuan << "\n";
                cout << "  Berat    : " << found->berat << " kg\n";
                cout << "  Biaya    : Rp " << found->biaya << "\n";
                cout << "  Status   : " << found->status << "\n";
            } else {
                cout << "  [ERROR] Paket tidak ditemukan!\n";
            }
            break;
        }
        case 5: { 
            string resi;
            cout << "\n  Masukkan nomor resi: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, resi);
            Paket* found = paket.findPaketByResi(resi);
            if (found) {
                cout << "\n  ===== PAKET DITEMUKAN =====\n";
                cout << "  ID       : " << found->id << "\n";
                cout << "  Resi     : " << found->resi << "\n";
                cout << "  Penerima : " << found->nama_penerima << "\n";
                cout << "  Status   : " << found->status << "\n";
            } else {
                cout << "  [ERROR] Paket tidak ditemukan!\n";
            }
            break;
        }
        case 6: paket.displayAllPaket(); break;
        case 7: paket.displayQueue(); break;
        case 8: paket.displayKurir(); break;
        case 9: {
            auto layananList = paket.getLayanan();
            cout << "\n  ===== DAFTAR LAYANAN =====\n";
            for (const auto& l : layananList) {
                cout << "  ID: " << l.id << " | " << l.nama << " | Tarif: Rp " << l.tarif_per_kg << "/kg\n";
            }
            break;
        }
        case 10: {
            auto klasList = paket.getKlasifikasi();
            cout << "\n  ===== KLASIFIKASI BERAT =====\n";
            for (const auto& k : klasList) {
                cout << "  ID: " << k.id << " | " << k.nama << " | Tambahan: Rp " << k.biaya_tambahan << "\n";
            }
            break;
        }
        case 11: paket.displayAVLTree(); break;
        case 0: cout << "  Logout...\n"; return true;
        case 99: cout << "\n  Keluar dari program...\n"; return false;
        default: cout << "  Pilihan tidak valid!\n"; break;
        }
    } while (true);
}

} 