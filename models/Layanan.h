#pragma once
#include <string>
using namespace std;

struct Layanan {
    int id;
    string nama;           
    double tarif_per_kg;        

    Layanan(int id = 0,
            const string& nama = "",
            double tarif_per_kg = 0.0)
        : id(id), nama(nama), tarif_per_kg(tarif_per_kg) {}
};