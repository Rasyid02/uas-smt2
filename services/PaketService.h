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
using namespace std;

namespace SwiftExpedition {  

class PaketService {
private:

    SinglyLinkedList<Paket> paketList;

    AVLTree<string, int> resiTree;

    Queue<Paket> paketQueue;

    CircularLinkedList<Kurir> kurirList;

    vector<Layanan> layananList;
    vector<KlasifikasiBerat> klasifikasiList;

    int nextPaketId;
    string generateResi();  

public:
    PaketService();

    void loadPaket(const string& filename);
    void loadLayanan(const string& filename);
    void loadKlasifikasi(const string& filename);
    void loadKurir(const string& filename);

    void addPaket(const Paket& p);
    bool deletePaket(int id);
    bool editPaket(int id, const Paket& newData);

    Paket* findPaketById(int id);      
    Paket* findPaketByResi(const string& resi);

    vector<Paket> sortByBiaya(bool ascending = true);
    vector<Paket> sortByBerat(bool ascending = true);

    int countByStatus(const string& status);

    void enqueuePaket(const Paket& p);
    bool dequeuePaket(Paket& result);

    Kurir* getNextKurir();     
    void assignPaketToKurir(int paketId, int kurirId);

    void displayAllPaket() const;
    void displayQueue() const;
    void displayKurir() const;
    void displayAVLTree() const;

    void savePaket(const string& filename);

    int getPaketCount() const;
    int getQueueSize() const;
    vector<Paket> getAllPaket() const;
    vector<Layanan> getLayanan() const;
    vector<KlasifikasiBerat> getKlasifikasi() const;
    string getLayananName(int id) const;
    string getKlasifikasiName(int id) const;
    double getLayananTarif(int id) const;
    double getKlasifikasiBiaya(int id) const;
};

} 