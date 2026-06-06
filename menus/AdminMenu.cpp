#include "AdminMenu.h"
#include "../services/PaketService.h"
#include <iostream>
#include <limits>
#include <string>

namespace SwiftExpedition {

bool AdminMenu::show(User* user, PaketService& paket) {
    int pilihan;
    do {
        std::cout << "\n  ============================================\n";
        std::cout << "  === MENU ADMIN ===\n";
        std::cout << "  User: " << user->nama << "\n";
        std::cout << "  ============================================\n";
        std::cout << "  1. Tambah Paket\n";
        std::cout << "  2. Hapus Paket\n";
        std::cout << "  3. Edit Paket\n";
        std::cout << "  4. Cari Paket (by ID)\n";
        std::cout << "  5. Cari Paket (by Resi)\n";
        std::cout << "  6. Lihat Semua Paket\n";
        std::cout << "  7. Lihat Antrean Paket\n";
        std::cout << "  8. Lihat Daftar Kurir\n";
        std::cout << "  9. Lihat Daftar Layanan\n";
        std::cout << "  10. Lihat Klasifikasi Berat\n";
        std::cout << "  11. Lihat AVL Tree (Resi)\n";
        std::cout << "  0. Logout\n";
        std::cout << "  99. Exit Program\n";
        std::cout << "  ----------------------------------------\n";
        std::cout << "  Pilihan: ";
        std::cin >> pilihan;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (pilihan) {
        case 1: { // Tambah paket
            Paket p;
            p.id = 0; // akan di-generate oleh service
            std::cout << "\n  --- Tambah Paket Baru ---\n";
            std::cout << "  Resi          : "; std::getline(std::cin, p.resi);
            std::cout << "  Nama Penerima : "; std::getline(std::cin, p.nama_penerima);
            std::cout << "  Alamat Tujuan : "; std::getline(std::cin, p.alamat_tujuan);
            std::cout << "  Kota Asal     : "; std::getline(std::cin, p.kota_asal);
            std::cout << "  Kota Tujuan   : "; std::getline(std::cin, p.kota_tujuan);
            std::cout << "  Berat (kg)    : "; std::cin >> p.berat;
            std::cout << "  ID Layanan    : "; std::cin >> p.id_layanan;
            std::cout << "  ID Klasifikasi: "; std::cin >> p.id_klasifikasi;
            std::cout << "  Biaya         : "; std::cin >> p.biaya;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            p.status = "Pending";
            p.id_kurir = 0;
            paket.addPaket(p);
            // Juga masukkan ke queue
            paket.enqueuePaket(p);
            std::cout << "\n  [SUCCESS] Paket berhasil ditambahkan dan dimasukkan ke antrean!\n";
            break;
        }
        case 2: { // Hapus paket
            int id;
            std::cout << "\n  Masukkan ID paket yang akan dihapus: ";
            std::cin >> id;
            if (paket.deletePaket(id)) {
                std::cout << "  [SUCCESS] Paket berhasil dihapus!\n";
            } else {
                std::cout << "  [ERROR] Paket tidak ditemukan!\n";
            }
            break;
        }
        case 3: { // Edit paket
            int id;
            std::cout << "\n  Masukkan ID paket yang akan diedit: ";
            std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            Paket* existing = paket.findPaketById(id);
            if (!existing) {
                std::cout << "  [ERROR] Paket tidak ditemukan!\n";
                break;
            }

            Paket newData = *existing;
            std::cout << "  --- Edit Paket (kosongkan untuk tidak mengubah) ---\n";
            std::string input;

            std::cout << "  Resi [" << newData.resi << "]: "; std::getline(std::cin, input);
            if (!input.empty()) newData.resi = input;

            std::cout << "  Nama Penerima [" << newData.nama_penerima << "]: "; std::getline(std::cin, input);
            if (!input.empty()) newData.nama_penerima = input;

            std::cout << "  Alamat Tujuan [" << newData.alamat_tujuan << "]: "; std::getline(std::cin, input);
            if (!input.empty()) newData.alamat_tujuan = input;

            std::cout << "  Kota Asal [" << newData.kota_asal << "]: "; std::getline(std::cin, input);
            if (!input.empty()) newData.kota_asal = input;

            std::cout << "  Kota Tujuan [" << newData.kota_tujuan << "]: "; std::getline(std::cin, input);
            if (!input.empty()) newData.kota_tujuan = input;

            std::cout << "  Status [" << newData.status << "]: "; std::getline(std::cin, input);
            if (!input.empty()) newData.status = input;

            std::cout << "  Berat [" << newData.berat << "]: "; std::getline(std::cin, input);
            if (!input.empty()) newData.berat = std::stod(input);

            std::cout << "  Biaya [" << newData.biaya << "]: "; std::getline(std::cin, input);
            if (!input.empty()) newData.biaya = std::stod(input);

            if (paket.editPaket(id, newData)) {
                std::cout << "  [SUCCESS] Paket berhasil diupdate!\n";
            } else {
                std::cout << "  [ERROR] Gagal mengupdate paket!\n";
            }
            break;
        }
        case 4: { // Cari by ID
            int id;
            std::cout << "\n  Masukkan ID paket: ";
            std::cin >> id;
            Paket* found = paket.findPaketById(id);
            if (found) {
                std::cout << "\n  ===== PAKET DITEMUKAN =====\n";
                std::cout << "  ID       : " << found->id << "\n";
                std::cout << "  Resi     : " << found->resi << "\n";
                std::cout << "  Penerima : " << found->nama_penerima << "\n";
                std::cout << "  Alamat   : " << found->alamat_tujuan << "\n";
                std::cout << "  Dari     : " << found->kota_asal << " -> " << found->kota_tujuan << "\n";
                std::cout << "  Berat    : " << found->berat << " kg\n";
                std::cout << "  Biaya    : Rp " << found->biaya << "\n";
                std::cout << "  Status   : " << found->status << "\n";
            } else {
                std::cout << "  [ERROR] Paket tidak ditemukan!\n";
            }
            break;
        }
        case 5: { // Cari by Resi
            std::string resi;
            std::cout << "\n  Masukkan nomor resi: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, resi);
            Paket* found = paket.findPaketByResi(resi);
            if (found) {
                std::cout << "\n  ===== PAKET DITEMUKAN =====\n";
                std::cout << "  ID       : " << found->id << "\n";
                std::cout << "  Resi     : " << found->resi << "\n";
                std::cout << "  Penerima : " << found->nama_penerima << "\n";
                std::cout << "  Status   : " << found->status << "\n";
            } else {
                std::cout << "  [ERROR] Paket tidak ditemukan!\n";
            }
            break;
        }
        case 6: paket.displayAllPaket(); break;
        case 7: paket.displayQueue(); break;
        case 8: paket.displayKurir(); break;
        case 9: {
            auto layananList = paket.getLayanan();
            std::cout << "\n  ===== DAFTAR LAYANAN =====\n";
            for (const auto& l : layananList) {
                std::cout << "  ID: " << l.id << " | " << l.nama << " | Tarif: Rp " << l.tarif_per_kg << "/kg\n";
            }
            break;
        }
        case 10: {
            auto klasList = paket.getKlasifikasi();
            std::cout << "\n  ===== KLASIFIKASI BERAT =====\n";
            for (const auto& k : klasList) {
                std::cout << "  ID: " << k.id << " | " << k.nama << " | Tambahan: Rp " << k.biaya_tambahan << "\n";
            }
            break;
        }
        case 11: paket.displayAVLTree(); break;
        case 0: std::cout << "  Logout...\n"; return true;
        case 99: std::cout << "\n  Keluar dari program...\n"; return false;
        default: std::cout << "  Pilihan tidak valid!\n"; break;
        }
    } while (true);
}

} // namespace SwiftExpedition