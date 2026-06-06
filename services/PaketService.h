#pragma once
#include <string>
#include <vector>
#include "../models/Paket.h"
#include "../models/Layanan.h"
#include "../models/KlasifikasiBerat.h"
#include "../models/Kurir.h"
#include "../datastructures/SinglyLinkedList.h"
#include "../datastructures/AVLTree.h"
#include "../datastructures/Queue.h"
#include "../datastructures/CircularLinkedList.h"

// ============================================================
// UAS: PaketService - CRUD dan manajemen paket
// Menggunakan SinglyLinkedList (Struktur Data 1) untuk master data
// Menggunakan AVLTree (Struktur Data 5) untuk pencarian resi
// Menggunakan Queue (Struktur Data 4) untuk antrean paket
// Menggunakan CircularLinkedList (Struktur Data 2) untuk rotasi kurir
// Menggunakan sort, find, count (STL) (12)(13)(14)
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

class PaketService {
private:
    // UAS: SinglyLinkedList (1) - Master data paket
    SinglyLinkedList<Paket> paketList;

    // UAS: AVLTree (5) - Pencarian nomor resi
    AVLTree<std::string, int> resiTree;

    // UAS: Queue (4) - Antrean paket masuk
    Queue<Paket> paketQueue;

    // UAS: CircularLinkedList (2) - Rotasi kurir
    CircularLinkedList<Kurir> kurirList;

    // Data pendukung dari CSV
    std::vector<Layanan> layananList;
    std::vector<KlasifikasiBerat> klasifikasiList;

    // Counter ID
    int nextPaketId;
    std::string generateResi();  // Generate nomor resi unik

public:
    PaketService();

    // ========== LOAD DATA ==========
    // UAS: file handling (15)
    void loadPaket(const std::string& filename);
    void loadLayanan(const std::string& filename);
    void loadKlasifikasi(const std::string& filename);
    void loadKurir(const std::string& filename);

    // ========== CRUD PAKET ==========
    void addPaket(const Paket& p);
    bool deletePaket(int id);
    bool editPaket(int id, const Paket& newData);

    // UAS: find (13) - Mencari paket
    Paket* findPaketById(int id);      // UAS: pointer (*) - 2
    Paket* findPaketByResi(const std::string& resi);

    // UAS: sort (12) - Sorting paket
    std::vector<Paket> sortByBiaya(bool ascending = true);
    std::vector<Paket> sortByBerat(bool ascending = true);

    // UAS: count (14) - Menghitung paket berdasarkan status
    int countByStatus(const std::string& status);

    // ========== QUEUE ==========
    void enqueuePaket(const Paket& p);
    bool dequeuePaket(Paket& result);

    // ========== KURIR ROTASI ==========
    Kurir* getNextKurir();     // UAS: CircularLinkedList (2) - Rotasi kurir
    void assignPaketToKurir(int paketId, int kurirId);

    // ========== DISPLAY ==========
    void displayAllPaket() const;
    void displayQueue() const;
    void displayKurir() const;
    void displayAVLTree() const;

    // ========== SAVE DATA ==========
    // UAS: file handling (15)
    void savePaket(const std::string& filename);

    // ========== GETTERS ==========
    int getPaketCount() const;
    int getQueueSize() const;
    std::vector<Paket> getAllPaket() const;
    std::vector<Layanan> getLayanan() const;
    std::vector<KlasifikasiBerat> getKlasifikasi() const;
    std::string getLayananName(int id) const;
    std::string getKlasifikasiName(int id) const;
    double getLayananTarif(int id) const;
    double getKlasifikasiBiaya(int id) const;
};

} // namespace SwiftExpedition