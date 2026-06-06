#pragma once
#include <string>
using namespace std;

struct Tracking {
    int id;
    int id_paket;               
    string lokasi;         
    string status;         
    string timestamp;      
    string keterangan;     

    Tracking(int id = 0,
             int id_paket = 0,
             const string& lokasi = "",
             const string& status = "",
             const string& timestamp = "",
             const string& keterangan = "")
        : id(id), id_paket(id_paket), lokasi(lokasi),
          status(status), timestamp(timestamp), keterangan(keterangan) {}
};