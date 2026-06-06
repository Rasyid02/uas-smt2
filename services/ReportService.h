#pragma once
#include <vector>
#include <string>
#include "../models/Paket.h"

// ============================================================
// UAS: ReportService - Laporan dan statistik
// UAS: sort (12), count (14), callback (5), lambda (16)
// ============================================================

namespace SwiftExpedition {

class ReportService {
public:
    ReportService();

    void displayLaporanPaket(std::vector<Paket>& pakets);
    void displayLaporanByStatus(std::vector<Paket>& pakets);
    void displayStatistik(std::vector<Paket>& pakets);

    void displayCEOReport(
        std::vector<Paket>& pakets,
        int totalKurir,
        int totalTracking
    );
};

} // namespace SwiftExpedition