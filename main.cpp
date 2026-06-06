
#include <iostream>
#include <limits>

#include "models/Role.h"
#include "models/User.h"
#include "models/Paket.h"
#include "models/Tracking.h"
#include "models/Kurir.h"
#include "models/Layanan.h"
#include "models/KlasifikasiBerat.h"
#include "models/Kota.h"

#include "datastructures/SinglyLinkedList.h"
#include "datastructures/CircularLinkedList.h"
#include "datastructures/Stack.h"
#include "datastructures/Queue.h"
#include "datastructures/AVLTree.h"
#include "datastructures/Graph.h"
#include "datastructures/HashTable.h"

#include "storage/CSVManager.h"

#include "services/AuthService.h"
#include "services/PaketService.h"
#include "services/TrackingService.h"
#include "services/ReportService.h"
#include "services/RoutingService.h"

#include "menus/LoginMenu.h"
#include "menus/AdminMenu.h"
#include "menus/KurirMenu.h"
#include "menus/ManagerMenu.h"
#include "menus/CeoMenu.h"
using namespace std;

using namespace SwiftExpedition;

int main() {

    AuthService auth;
    PaketService paket;
    TrackingService tracking;
    ReportService report;
    RoutingService routing;

    cout << "\n  ============================================\n";
    cout << "  === LOADING DATA DARI CSV... ===\n";
    cout << "  ============================================\n";

    auth.loadUsers("anggota.csv");

    paket.loadLayanan("layanan.csv");
    paket.loadKlasifikasi("klasifikasi.csv");
    paket.loadPaket("paket.csv");
    paket.loadKurir("kurir.csv");  

    tracking.loadTracking("tracking.csv");

    routing.loadKota("kota.csv");

    cout << "\n  ============================================\n";
    cout << "  === SEMUA DATA BERHASIL DIMUAT ===\n";
    cout << "  ============================================\n";

    LoginMenu loginMenu;
    AdminMenu adminMenu;
    KurirMenu kurirMenu;
    ManagerMenu managerMenu;
    CeoMenu ceoMenu;

    User* currentUser = nullptr;

    do {
        currentUser = loginMenu.show(auth);

        if (currentUser) {

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
                    cout << "\n  [ERROR] Role tidak dikenali.\n";
                    break;
            }

            cout << "\n\n  ============================================\n";
            cout << "  === MENYIMPAN DATA... ===\n";
            paket.savePaket("paket.csv");
            tracking.saveTracking("tracking.csv");
            cout << "  ============================================\n";

            if (!continueProgram) {
                break;
            }
        }

    } while (true); 

    return 0;
}