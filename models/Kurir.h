#pragma once
#include <string>
using namespace std;

struct Kurir {
    int id;
    string nama;
    string status;         
    int total_paket;            

    Kurir(int id = 0,
          const string& nama = "",
          const string& status = "Tersedia",
          int total_paket = 0)
        : id(id), nama(nama), status(status), total_paket(total_paket) {}
};