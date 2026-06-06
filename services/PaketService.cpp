#include "PaketService.h"
#include "../storage/CSVManager.h"
#include <iostream>
#include <algorithm>  
#include <ctime>
using namespace std;

namespace SwiftExpedition {

PaketService::PaketService() : nextPaketId(1) {
}

string PaketService::generateResi() {

    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[30];
    sprintf(buffer, "SWF-%04d%02d%02d-%04d",
            1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, nextPaketId);
    return string(buffer);
}

void PaketService::loadPaket(const string& filename) {
    try {
        vector<string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            vector<string> t = CSVManager::split(lines[i], ';');

            if (t.size() >= 12) {
                Paket p(
                    stoi(t[0]),      
                    t[1],                 
                    t[2],                 
                    t[3],                 
                    t[4],                 
                    t[5],                 
                    stod(t[6]),      
                    stod(t[7]),      
                    t[8],                 
                    stoi(t[9]),      
                    stoi(t[10]),     
                    stoi(t[11])      
                );
                paketList.insert(p);                  
                resiTree.insert(p.resi, p.id);        

                if (p.id >= nextPaketId) {
                    nextPaketId = p.id + 1;
                }

                auto isMenunggu = [](const string& s) { return s == "menunggu"; };
                if (isMenunggu(p.status)) {
                    paketQueue.enqueue(p);  
                }
            }
        }
        cout << "  [LOAD] " << paketList.size() << " paket dimuat dari " << filename << ".\n";
    }
    catch (const exception& e) {
        cout << "  [ERROR] Gagal memuat paket: " << e.what() << "\n";
    }
}

void PaketService::loadLayanan(const string& filename) {
    try {
        vector<string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            vector<string> t = CSVManager::split(lines[i], ';');
            if (t.size() >= 3) {
                Layanan l(stoi(t[0]), t[1], stod(t[2]));
                layananList.push_back(l);
            }
        }
        cout << "  [LOAD] " << layananList.size() << " layanan dimuat.\n";
    }
    catch (const exception& e) {
        cout << "  [ERROR] Gagal memuat layanan: " << e.what() << "\n";
    }
}

void PaketService::loadKlasifikasi(const string& filename) {
    try {
        vector<string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            vector<string> t = CSVManager::split(lines[i], ';');
            if (t.size() >= 3) {
                KlasifikasiBerat k(stoi(t[0]), t[1], stod(t[2]));
                klasifikasiList.push_back(k);
            }
        }
        cout << "  [LOAD] " << klasifikasiList.size() << " klasifikasi dimuat.\n";
    }
    catch (const exception& e) {
        cout << "  [ERROR] Gagal memuat klasifikasi: " << e.what() << "\n";
    }
}

void PaketService::loadKurir(const string& filename) {

    try {
        vector<string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            vector<string> t = CSVManager::split(lines[i], ';');
            if (t.size() >= 4) {
                Kurir k(stoi(t[0]), t[1], t[2], stoi(t[3]));
                kurirList.insert(k);  
            }
        }
        cout << "  [LOAD] " << kurirList.size() << " kurir dimuat dari " << filename << ".\n";
    }
    catch (const exception& e) {
        cout << "  [WARNING] Gagal memuat kurir: " << e.what() << "\n";
        cout << "  [INFO] Menambahkan kurir default...\n";

        kurirList.insert(Kurir(1, "Budi Santoso", "Tersedia", 0));
        kurirList.insert(Kurir(2, "Andi Pratama", "Tersedia", 0));
        kurirList.insert(Kurir(3, "Citra Dewi", "Tersedia", 0));
    }
}

void PaketService::addPaket(const Paket& p) {
    Paket newPaket = p;
    newPaket.id = nextPaketId++;
    if (newPaket.resi.empty()) {
        char resiBuf[30];
        time_t now = time(0);
        tm* ltm = localtime(&now);
        sprintf(resiBuf, "SWF-%04d%02d%02d-%04d",
                1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, newPaket.id);
        newPaket.resi = string(resiBuf);
    }

    paketList.insert(newPaket);             
    resiTree.insert(newPaket.resi, newPaket.id);  
    paketQueue.enqueue(newPaket);           

    cout << "  [SUKSES] Paket " << newPaket.resi << " berhasil ditambahkan.\n";
}

bool PaketService::deletePaket(int id) {

    Paket* p = paketList.findById(id);
    if (!p) {
        cout << "  [GAGAL] Paket dengan ID " << id << " tidak ditemukan.\n";
        return false;
    }
    string resi = p->resi;
    paketList.removeById(id);  

    cout << "  [SUKSES] Paket " << resi << " berhasil dihapus.\n";
    return true;
}

bool PaketService::editPaket(int id, const Paket& newData) {
    Paket updateData = newData;
    updateData.id = id;
    if (paketList.update(id, updateData)) {  
        cout << "  [SUKSES] Paket ID " << id << " berhasil diperbarui.\n";
        return true;
    }
    cout << "  [GAGAL] Paket dengan ID " << id << " tidak ditemukan.\n";
    return false;
}

Paket* PaketService::findPaketById(int id) {
    return paketList.findById(id);
}

Paket* PaketService::findPaketByResi(const string& resi) {

    int* idPtr = resiTree.search(resi);
    if (idPtr) {
        return paketList.findById(*idPtr);
    }
    return nullptr;
}

vector<Paket> PaketService::sortByBiaya(bool ascending) {
    vector<Paket> result = paketList.toVector();

    sort(result.begin(), result.end(),
        [ascending](const Paket& a, const Paket& b) {
            return ascending ? (a.biaya < b.biaya) : (a.biaya > b.biaya);
        });
    return result;
}

vector<Paket> PaketService::sortByBerat(bool ascending) {
    vector<Paket> result = paketList.toVector();
    sort(result.begin(), result.end(),
        [ascending](const Paket& a, const Paket& b) {
            return ascending ? (a.berat < b.berat) : (a.berat > b.berat);
        });
    return result;
}

int PaketService::countByStatus(const string& status) {
    vector<Paket> result = paketList.toVector();

    return count_if(result.begin(), result.end(),
        [&status](const Paket& p) { return p.status == status; });
}

void PaketService::enqueuePaket(const Paket& p) {
    paketQueue.enqueue(p);
}

bool PaketService::dequeuePaket(Paket& result) {
    return paketQueue.dequeue(result);
}

Kurir* PaketService::getNextKurir() {
    if (kurirList.isEmpty()) return nullptr;

    kurirList.rotate();
    return kurirList.current();
}

void PaketService::assignPaketToKurir(int paketId, int kurirId) {
    Paket* p = paketList.findById(paketId);
    if (p) {
        p->id_kurir = kurirId;
        p->status = "dalam_perjalanan";
        cout << "  [SUKSES] Paket " << p->resi << " diassign ke Kurir ID " << kurirId << ".\n";
    }
}

void PaketService::displayAllPaket() const {
    cout << "\n  ===== DAFTAR SEMUA PAKET (Singly Linked List) =====\n";
    cout << "  Total: " << paketList.size() << " paket\n";
    paketList.display();

    vector<Paket> pakets = paketList.toVector();
    if (!pakets.empty()) {
        cout << "\n  Detail:\n";
        for (size_t i = 0; i < pakets.size(); i++) {
            const Paket& p = pakets[i];
            cout << "  ----------------------------------------\n";
            cout << "  ID       : " << p.id << "\n";
            cout << "  Resi     : " << p.resi << "\n";
            cout << "  Penerima : " << p.nama_penerima << "\n";
            cout << "  Tujuan   : " << p.kota_asal << " -> " << p.kota_tujuan << "\n";
            cout << "  Alamat   : " << p.alamat_tujuan << "\n";
            cout << "  Berat    : " << p.berat << " kg\n";
            cout << "  Biaya    : Rp " << p.biaya << "\n";
            cout << "  Layanan  : " << getLayananName(p.id_layanan) << "\n";
            cout << "  Klasifikasi: " << getKlasifikasiName(p.id_klasifikasi) << "\n";
            cout << "  Kurir ID : " << p.id_kurir << "\n";
            cout << "  Status   : " << p.status << "\n";
        }
    }
}

void PaketService::displayQueue() const {
    cout << "\n  ===== ANTREAN PAKET (Queue) =====\n";
    cout << "  Jumlah antrean: " << paketQueue.size() << "\n";
    paketQueue.display();
}

void PaketService::displayKurir() const {
    cout << "\n  ===== DAFTAR KURIR (Circular Linked List) =====\n";
    cout << "  Total: " << kurirList.size() << " kurir\n";
    kurirList.display();
}

void PaketService::displayAVLTree() const {
    cout << "\n  ===== PENCARIAN RESI (AVL Tree) =====\n";
    resiTree.display();
}

void PaketService::savePaket(const string& filename) {
    vector<Paket> pakets = paketList.toVector();
    vector<string> lines;
    lines.push_back("id;resi;nama_penerima;alamat_tujuan;kota_asal;kota_tujuan;berat;biaya;status;id_layanan;id_klasifikasi;id_kurir");
    for (const Paket& p : pakets) {
        vector<string> row = {
            to_string(p.id), p.resi, p.nama_penerima,
            p.alamat_tujuan, p.kota_asal, p.kota_tujuan,
            to_string(p.berat), to_string(p.biaya),
            p.status, to_string(p.id_layanan),
            to_string(p.id_klasifikasi), to_string(p.id_kurir)
        };
        lines.push_back(CSVManager::join(row, ';'));
    }
    CSVManager::write("data/" + filename, lines);
    cout << "  [SUKSES] " << pakets.size() << " paket disimpan ke " << filename << ".\n";
}

int PaketService::getPaketCount() const {
    return paketList.size();
}

int PaketService::getQueueSize() const {
    return paketQueue.size();
}

vector<Paket> PaketService::getAllPaket() const {
    return paketList.toVector();
}

vector<Layanan> PaketService::getLayanan() const {
    return layananList;
}

vector<KlasifikasiBerat> PaketService::getKlasifikasi() const {
    return klasifikasiList;
}

string PaketService::getLayananName(int id) const {

    auto it = find_if(layananList.begin(), layananList.end(),
        [id](const Layanan& l) { return l.id == id; });
    return (it != layananList.end()) ? it->nama : "Tidak diketahui";
}

string PaketService::getKlasifikasiName(int id) const {
    auto it = find_if(klasifikasiList.begin(), klasifikasiList.end(),
        [id](const KlasifikasiBerat& k) { return k.id == id; });
    return (it != klasifikasiList.end()) ? it->nama : "Tidak diketahui";
}

double PaketService::getLayananTarif(int id) const {
    auto it = find_if(layananList.begin(), layananList.end(),
        [id](const Layanan& l) { return l.id == id; });
    return (it != layananList.end()) ? it->tarif_per_kg : 0.0;
}

double PaketService::getKlasifikasiBiaya(int id) const {
    auto it = find_if(klasifikasiList.begin(), klasifikasiList.end(),
        [id](const KlasifikasiBerat& k) { return k.id == id; });
    return (it != klasifikasiList.end()) ? it->biaya_tambahan : 0.0;
}

} 