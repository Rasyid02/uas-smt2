#pragma once
#include <string>
using namespace std;

struct Kota {
    string asal;           
    string tujuan;         
    int jarak;                  

    Kota(const string& asal = "",
         const string& tujuan = "",
         int jarak = 0)
        : asal(asal), tujuan(tujuan), jarak(jarak) {}
};