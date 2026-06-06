#pragma once
#include <vector>
#include <string>
#include "../models/Paket.h"
using namespace std;

namespace SwiftExpedition {

class ReportService {
public:
    ReportService();

    void displayLaporanPaket(vector<Paket>& pakets);
    void displayLaporanByStatus(vector<Paket>& pakets);
    void displayStatistik(vector<Paket>& pakets);

    void displayCEOReport(
        vector<Paket>& pakets,
        int totalKurir,
        int totalTracking
    );
};

} 