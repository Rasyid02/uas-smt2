// ============================================================
// UAS: SISTEM MANAJEMEN EKSPEDISI - SWIFT EXPEDITION
// Algoritma Pemrograman & Struktur Data
// 
// Fitur lengkap dengan integrasi seluruh struktur data wajib
// ============================================================
#include <iostream>
#include <limits>

// Models (8 files)
#include "models/Role.h"
#include "models/User.h"
#include "models/Paket.h"
#include "models/Tracking.h"
#include "models/Kurir.h"
#include "models/Layanan.h"
#include "models/KlasifikasiBerat.h"
#include "models/Kota.h"

// Data Structures (7 files)
#include "datastructures/SinglyLinkedList.h"
#include "datastructures/CircularLinkedList.h"
#include "datastructures/Stack.h"
#include "datastructures/Queue.h"
#include "datastructures/AVLTree.h"
#include "datastructures/Graph.h"
#include "datastructures/HashTable.h"

// Storage
#include "storage/CSVManager.h"

// Services
#include "services/AuthService.h"
#include "services/PaketService.h"
#include "services/TrackingService.h"
#include "services/ReportService.h"
#include "services/RoutingService.h"

// Menus
#include "menus/LoginMenu.h"
#include "menus/AdminMenu.h"
#include "menus/KurirMenu.h"
#include "menus/ManagerMenu.h"
#include "menus/CeoMenu.h"

using namespace SwiftExpedition;

// ============================================================
// MAIN PROGRAM
// ============================================================
int main() {
    // -------- Inisialisasi Services --------
    AuthService auth;
    PaketService paket;
    TrackingService tracking;
    ReportService report;
    RoutingService routing;

    // -------- Load Semua Data dari CSV --------
    std::cout << "\n  ============================================\n";
    std::cout << "  === LOADING DATA DARI CSV... ===\n";
    std::cout << "  ============================================\n";

    auth.loadUsers("anggota.csv");

    paket.loadLayanan("layanan.csv");
    paket.loadKlasifikasi("klasifikasi.csv");
    paket.loadPaket("paket.csv");
    paket.loadKurir("kurir.csv");  // Load kurir dari file terpisah

    tracking.loadTracking("tracking.csv");

    routing.loadKota("kota.csv");

    std::cout << "\n  ============================================\n";
    std::cout << "  === SEMUA DATA BERHASIL DIMUAT ===\n";
    std::cout << "  ============================================\n";

    // -------- Login Menu --------
    LoginMenu loginMenu;
    AdminMenu adminMenu;
    KurirMenu kurirMenu;
    ManagerMenu managerMenu;
    CeoMenu ceoMenu;

    User* currentUser = nullptr;

    do {
        currentUser = loginMenu.show(auth);

        if (currentUser) {
            // Route berdasarkan role
            bool continueProgram = true;
            switch (currentUser->role) {
                case RoleType::ADMIN:
                    continueProgram = adminMenu.show(currentUser, paket);
                    break;

                case RoleType::KURIR:
                    continueProgram = kurirMenu.show(currentUser, paket, tracking);
                    break;

                case RoleType::MANAGER:
                    continueProgram = managerMenu.show(currentUser, paket, report, tracking);
                    break;

                case RoleType::CEO:
                    continueProgram = ceoMenu.show(currentUser, paket, report, tracking, routing);
                    break;

                default:
                    std::cout << "\n  [ERROR] Role tidak dikenali.\n";
                    break;
            }

            // -------- Simpan data setelah logout --------
            std::cout << "\n\n  ============================================\n";
            std::cout << "  === MENYIMPAN DATA... ===\n";
            paket.savePaket("paket.csv");
            tracking.saveTracking("tracking.csv");
            std::cout << "  ============================================\n";

            // Jika user memilih Exit, keluar dari program
            if (!continueProgram) {
                break;
            }
        }

    } while (true); // Loop terus untuk multi-user login

    return 0;
}