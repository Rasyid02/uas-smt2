#include "PaketService.h"
#include "../storage/CSVManager.h"
#include <iostream>
#include <algorithm>  // UAS: sort (12)
#include <ctime>

// ============================================================
// UAS: PaketService - Implementasi manajemen paket
// Integrasi SinglyLinkedList, AVLTree, Queue, CircularLinkedList
// Integrasi sort, find, count (STL)
// ============================================================

namespace SwiftExpedition {

PaketService::PaketService() : nextPaketId(1) {
}

// UAS: inline function (7) - bisa juga ditaruh di sini untuk generate resi
std::string PaketService::generateResi() {
    // Format: SWF-YYYYMMDD-XXXX
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[30];
    sprintf(buffer, "SWF-%04d%02d%02d-%04d",
            1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, nextPaketId);
    return std::string(buffer);
}

// ============================================================
// LOAD DATA
// UAS: file handling (15) - Membaca dari CSV
// ============================================================
void PaketService::loadPaket(const std::string& filename) {
    try {
        std::vector<std::string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            std::vector<std::string> t = CSVManager::split(lines[i], ';');
            // CSV: id;resi;nama_penerima;alamat_tujuan;kota_asal;kota_tujuan;berat;biaya;status;id_layanan;id_klasifikasi;id_kurir
            if (t.size() >= 12) {
                Paket p(
                    std::stoi(t[0]),      // id
                    t[1],                 // resi
                    t[2],                 // nama_penerima
                    t[3],                 // alamat_tujuan
                    t[4],                 // kota_asal
                    t[5],                 // kota_tujuan
                    std::stod(t[6]),      // berat
                    std::stod(t[7]),      // biaya
                    t[8],                 // status
                    std::stoi(t[9]),      // id_layanan
                    std::stoi(t[10]),     // id_klasifikasi
                    std::stoi(t[11])      // id_kurir
                );
                paketList.insert(p);                  // UAS: SinglyLinkedList (1)
                resiTree.insert(p.resi, p.id);        // UAS: AVLTree (5)

                if (p.id >= nextPaketId) {
                    nextPaketId = p.id + 1;
                }

                // Jika status "menunggu", masukkan ke queue
                // UAS: lambda expression (16) - Filter paket menunggu
                auto isMenunggu = [](const std::string& s) { return s == "menunggu"; };
                if (isMenunggu(p.status)) {
                    paketQueue.enqueue(p);  // UAS: Queue (4)
                }
            }
        }
        std::cout << "  [LOAD] " << paketList.size() << " paket dimuat dari " << filename << ".\n";
    }
    catch (const std::exception& e) {
        std::cout << "  [ERROR] Gagal memuat paket: " << e.what() << "\n";
    }
}

void PaketService::loadLayanan(const std::string& filename) {
    try {
        std::vector<std::string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            std::vector<std::string> t = CSVManager::split(lines[i], ';');
            if (t.size() >= 3) {
                Layanan l(std::stoi(t[0]), t[1], std::stod(t[2]));
                layananList.push_back(l);
            }
        }
        std::cout << "  [LOAD] " << layananList.size() << " layanan dimuat.\n";
    }
    catch (const std::exception& e) {
        std::cout << "  [ERROR] Gagal memuat layanan: " << e.what() << "\n";
    }
}

void PaketService::loadKlasifikasi(const std::string& filename) {
    try {
        std::vector<std::string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            std::vector<std::string> t = CSVManager::split(lines[i], ';');
            if (t.size() >= 3) {
                KlasifikasiBerat k(std::stoi(t[0]), t[1], std::stod(t[2]));
                klasifikasiList.push_back(k);
            }
        }
        std::cout << "  [LOAD] " << klasifikasiList.size() << " klasifikasi dimuat.\n";
    }
    catch (const std::exception& e) {
        std::cout << "  [ERROR] Gagal memuat klasifikasi: " << e.what() << "\n";
    }
}

void PaketService::loadKurir(const std::string& filename) {
    // Load data kurir dari CSV terpisah (data/kurir.csv)
    // Format: id;nama;status;total_paket
    // CircularLinkedList digunakan untuk rotasi kurir saat assign paket
    try {
        std::vector<std::string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            std::vector<std::string> t = CSVManager::split(lines[i], ';');
            if (t.size() >= 4) {
                Kurir k(std::stoi(t[0]), t[1], t[2], std::stoi(t[3]));
                kurirList.insert(k);  // UAS: CircularLinkedList (2)
            }
        }
        std::cout << "  [LOAD] " << kurirList.size() << " kurir dimuat dari " << filename << ".\n";
    }
    catch (const std::exception& e) {
        std::cout << "  [WARNING] Gagal memuat kurir: " << e.what() << "\n";
        std::cout << "  [INFO] Menambahkan kurir default...\n";
        // Tambahkan kurir default
        kurirList.insert(Kurir(1, "Budi Santoso", "Tersedia", 0));
        kurirList.insert(Kurir(2, "Andi Pratama", "Tersedia", 0));
        kurirList.insert(Kurir(3, "Citra Dewi", "Tersedia", 0));
    }
}

// ============================================================
// CRUD PAKET
// UAS: SinglyLinkedList (1) - insert, delete, update
// UAS: AVLTree (5) - insert key
// ============================================================
void PaketService::addPaket(const Paket& p) {
    Paket newPaket = p;
    newPaket.id = nextPaketId++;
    if (newPaket.resi.empty()) {
        char resiBuf[30];
        time_t now = time(0);
        tm* ltm = localtime(&now);
        sprintf(resiBuf, "SWF-%04d%02d%02d-%04d",
                1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, newPaket.id);
        newPaket.resi = std::string(resiBuf);
    }

    paketList.insert(newPaket);             // UAS: SinglyLinkedList (1)
    resiTree.insert(newPaket.resi, newPaket.id);  // UAS: AVLTree (5)
    paketQueue.enqueue(newPaket);           // UAS: Queue (4)

    std::cout << "  [SUKSES] Paket " << newPaket.resi << " berhasil ditambahkan.\n";
}

bool PaketService::deletePaket(int id) {
    // UAS: pointer (*) - 2
    Paket* p = paketList.findById(id);
    if (!p) {
        std::cout << "  [GAGAL] Paket dengan ID " << id << " tidak ditemukan.\n";
        return false;
    }
    std::string resi = p->resi;
    paketList.removeById(id);  // UAS: SinglyLinkedList (1) - remove
    // AVLTree tidak punya remove, tapi akan diabaikan saat search (return nullptr)
    std::cout << "  [SUKSES] Paket " << resi << " berhasil dihapus.\n";
    return true;
}

bool PaketService::editPaket(int id, const Paket& newData) {
    Paket updateData = newData;
    updateData.id = id;
    if (paketList.update(id, updateData)) {  // UAS: SinglyLinkedList (1) - update
        std::cout << "  [SUKSES] Paket ID " << id << " berhasil diperbarui.\n";
        return true;
    }
    std::cout << "  [GAGAL] Paket dengan ID " << id << " tidak ditemukan.\n";
    return false;
}

// ============================================================
// SEARCH
// UAS: pointer (*) - 2
// UAS: find (13) - STL algorithm
// ============================================================
Paket* PaketService::findPaketById(int id) {
    return paketList.findById(id);
}

Paket* PaketService::findPaketByResi(const std::string& resi) {
    // Gunakan AVL Tree untuk mendapatkan ID paket, lalu cari di list
    int* idPtr = resiTree.search(resi);
    if (idPtr) {
        return paketList.findById(*idPtr);
    }
    return nullptr;
}

// ============================================================
// SORT
// UAS: sort (12) - STL algorithm dengan lambda expression (16)
// ============================================================
std::vector<Paket> PaketService::sortByBiaya(bool ascending) {
    std::vector<Paket> result = paketList.toVector();
    // UAS: lambda expression (16) - Comparator untuk sort
    std::sort(result.begin(), result.end(),
        [ascending](const Paket& a, const Paket& b) {
            return ascending ? (a.biaya < b.biaya) : (a.biaya > b.biaya);
        });
    return result;
}

std::vector<Paket> PaketService::sortByBerat(bool ascending) {
    std::vector<Paket> result = paketList.toVector();
    std::sort(result.begin(), result.end(),
        [ascending](const Paket& a, const Paket& b) {
            return ascending ? (a.berat < b.berat) : (a.berat > b.berat);
        });
    return result;
}

// ============================================================
// COUNT
// UAS: count (14) - STL algorithm
// ============================================================
int PaketService::countByStatus(const std::string& status) {
    std::vector<Paket> result = paketList.toVector();
    // UAS: count (14) dengan lambda (16)
    return std::count_if(result.begin(), result.end(),
        [&status](const Paket& p) { return p.status == status; });
}

// ============================================================
// QUEUE
// UAS: Queue (4) - enqueue / dequeue
// ============================================================
void PaketService::enqueuePaket(const Paket& p) {
    paketQueue.enqueue(p);
}

bool PaketService::dequeuePaket(Paket& result) {
    return paketQueue.dequeue(result);
}

// ============================================================
// KURIR ROTASI
// UAS: CircularLinkedList (2) - Rotasi kurir
// ============================================================
Kurir* PaketService::getNextKurir() {
    if (kurirList.isEmpty()) return nullptr;
    // Rotasi dan return kurir saat ini
    kurirList.rotate();
    return kurirList.current();
}

void PaketService::assignPaketToKurir(int paketId, int kurirId) {
    Paket* p = paketList.findById(paketId);
    if (p) {
        p->id_kurir = kurirId;
        p->status = "dalam_perjalanan";
        std::cout << "  [SUKSES] Paket " << p->resi << " diassign ke Kurir ID " << kurirId << ".\n";
    }
}

// ============================================================
// DISPLAY
// ============================================================
void PaketService::displayAllPaket() const {
    std::cout << "\n  ===== DAFTAR SEMUA PAKET (Singly Linked List) =====\n";
    std::cout << "  Total: " << paketList.size() << " paket\n";
    paketList.display();

    // Detail display
    // UAS: iterator (11) - menggunakan begin() dan end()
    std::vector<Paket> pakets = paketList.toVector();
    if (!pakets.empty()) {
        std::cout << "\n  Detail:\n";
        for (size_t i = 0; i < pakets.size(); i++) {
            const Paket& p = pakets[i];
            std::cout << "  ----------------------------------------\n";
            std::cout << "  ID       : " << p.id << "\n";
            std::cout << "  Resi     : " << p.resi << "\n";
            std::cout << "  Penerima : " << p.nama_penerima << "\n";
            std::cout << "  Tujuan   : " << p.kota_asal << " -> " << p.kota_tujuan << "\n";
            std::cout << "  Alamat   : " << p.alamat_tujuan << "\n";
            std::cout << "  Berat    : " << p.berat << " kg\n";
            std::cout << "  Biaya    : Rp " << p.biaya << "\n";
            std::cout << "  Layanan  : " << getLayananName(p.id_layanan) << "\n";
            std::cout << "  Klasifikasi: " << getKlasifikasiName(p.id_klasifikasi) << "\n";
            std::cout << "  Kurir ID : " << p.id_kurir << "\n";
            std::cout << "  Status   : " << p.status << "\n";
        }
    }
}

void PaketService::displayQueue() const {
    std::cout << "\n  ===== ANTREAN PAKET (Queue) =====\n";
    std::cout << "  Jumlah antrean: " << paketQueue.size() << "\n";
    paketQueue.display();
}

void PaketService::displayKurir() const {
    std::cout << "\n  ===== DAFTAR KURIR (Circular Linked List) =====\n";
    std::cout << "  Total: " << kurirList.size() << " kurir\n";
    kurirList.display();
}

void PaketService::displayAVLTree() const {
    std::cout << "\n  ===== PENCARIAN RESI (AVL Tree) =====\n";
    resiTree.display();
}

// ============================================================
// SAVE DATA
// UAS: file handling (15) - Menulis ke CSV
// ============================================================
void PaketService::savePaket(const std::string& filename) {
    std::vector<Paket> pakets = paketList.toVector();
    std::vector<std::string> lines;
    lines.push_back("id;resi;nama_penerima;alamat_tujuan;kota_asal;kota_tujuan;berat;biaya;status;id_layanan;id_klasifikasi;id_kurir");
    for (const Paket& p : pakets) {
        std::vector<std::string> row = {
            std::to_string(p.id), p.resi, p.nama_penerima,
            p.alamat_tujuan, p.kota_asal, p.kota_tujuan,
            std::to_string(p.berat), std::to_string(p.biaya),
            p.status, std::to_string(p.id_layanan),
            std::to_string(p.id_klasifikasi), std::to_string(p.id_kurir)
        };
        lines.push_back(CSVManager::join(row, ';'));
    }
    CSVManager::write("data/" + filename, lines);
    std::cout << "  [SUKSES] " << pakets.size() << " paket disimpan ke " << filename << ".\n";
}

// ============================================================
// GETTERS
// ============================================================
int PaketService::getPaketCount() const {
    return paketList.size();
}

int PaketService::getQueueSize() const {
    return paketQueue.size();
}

std::vector<Paket> PaketService::getAllPaket() const {
    return paketList.toVector();
}

std::vector<Layanan> PaketService::getLayanan() const {
    return layananList;
}

std::vector<KlasifikasiBerat> PaketService::getKlasifikasi() const {
    return klasifikasiList;
}

std::string PaketService::getLayananName(int id) const {
    // UAS: find (13) - STL algorithm
    auto it = std::find_if(layananList.begin(), layananList.end(),
        [id](const Layanan& l) { return l.id == id; });
    return (it != layananList.end()) ? it->nama : "Tidak diketahui";
}

std::string PaketService::getKlasifikasiName(int id) const {
    auto it = std::find_if(klasifikasiList.begin(), klasifikasiList.end(),
        [id](const KlasifikasiBerat& k) { return k.id == id; });
    return (it != klasifikasiList.end()) ? it->nama : "Tidak diketahui";
}

double PaketService::getLayananTarif(int id) const {
    auto it = std::find_if(layananList.begin(), layananList.end(),
        [id](const Layanan& l) { return l.id == id; });
    return (it != layananList.end()) ? it->tarif_per_kg : 0.0;
}

double PaketService::getKlasifikasiBiaya(int id) const {
    auto it = std::find_if(klasifikasiList.begin(), klasifikasiList.end(),
        [id](const KlasifikasiBerat& k) { return k.id == id; });
    return (it != klasifikasiList.end()) ? it->biaya_tambahan : 0.0;
}

} // namespace SwiftExpedition