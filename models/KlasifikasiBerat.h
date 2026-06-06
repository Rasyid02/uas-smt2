#pragma once
#include <string>
using namespace std;

struct KlasifikasiBerat {
    int id;
    string nama;           
    double biaya_tambahan;      

    KlasifikasiBerat(int id = 0,
                     const string& nama = "",
                     double biaya_tambahan = 0.0)
        : id(id), nama(nama), biaya_tambahan(biaya_tambahan) {}
};