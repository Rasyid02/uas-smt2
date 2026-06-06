#pragma once
#include <string>
using namespace std;

struct Paket {
    int id;
    string resi;           
    string nama_penerima;
    string alamat_tujuan;
    string kota_asal;
    string kota_tujuan;
    double berat;               
    double biaya;               
    string status;         
    int id_layanan;             
    int id_klasifikasi;         
    int id_kurir;               

    Paket(int id = 0,
          const string& resi = "",
          const string& nama_penerima = "",
          const string& alamat_tujuan = "",
          const string& kota_asal = "",
          const string& kota_tujuan = "",
          double berat = 0.0,
          double biaya = 0.0,
          const string& status = "menunggu",
          int id_layanan = 0,
          int id_klasifikasi = 0,
          int id_kurir = 0)
        : id(id), resi(resi), nama_penerima(nama_penerima),
          alamat_tujuan(alamat_tujuan), kota_asal(kota_asal),
          kota_tujuan(kota_tujuan), berat(berat), biaya(biaya),
          status(status), id_layanan(id_layanan),
          id_klasifikasi(id_klasifikasi), id_kurir(id_kurir) {}
};