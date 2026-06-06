#include "TrackingService.h"
#include "../storage/CSVManager.h"
#include <iostream>
#include <ctime>

// ============================================================
// UAS: TrackingService - Implementasi tracking dengan Stack
// UAS: Stack (3) - Undo status tracking
// UAS: callback function (5) - Process tracking callbacks
// UAS: file handling (15)
// ============================================================

namespace SwiftExpedition {

TrackingService::TrackingService() {
}

// ============================================================
// LOAD
// UAS: file handling (15)
// ============================================================
void TrackingService::loadTracking(const std::string& filename) {
    try {
        std::vector<std::string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            std::vector<std::string> t = CSVManager::split(lines[i], ';');
            // CSV: id;id_paket;lokasi;status;timestamp
            if (t.size() >= 5) {
                Tracking tr(
                    std::stoi(t[0]),
                    std::stoi(t[1]),
                    t[2],   // lokasi
                    t[3],   // status
                    t[4]    // timestamp
                );
                trackingHistory.push_back(tr);
                // Push ke stack per paket untuk undo
                // UAS: Stack (3) - Push ke stack
                undoStacks[tr.id_paket].push(tr);
            }
        }
        std::cout << "  [LOAD] " << trackingHistory.size()
                  << " tracking history dimuat dari " << filename << ".\n";
    }
    catch (const std::exception& e) {
        std::cout << "  [WARNING] Gagal memuat tracking: " << e.what() << "\n";
    }
}

// ============================================================
// SAVE
// UAS: file handling (15)
// ============================================================
void TrackingService::saveTracking(const std::string& filename) {
    std::vector<std::string> lines;
    lines.push_back("id;id_paket;lokasi;status;timestamp");
    for (const Tracking& tr : trackingHistory) {
        std::vector<std::string> row = {
            std::to_string(tr.id),
            std::to_string(tr.id_paket),
            tr.lokasi,
            tr.status,
            tr.timestamp
        };
        lines.push_back(CSVManager::join(row, ';'));
    }
    CSVManager::write("data/" + filename, lines);
    std::cout << "  [SUKSES] " << trackingHistory.size()
              << " tracking history disimpan ke " << filename << ".\n";
}

// ============================================================
// UPDATE TRACKING
// UAS: Stack (3) - Setiap update didorong ke stack
// ============================================================
void TrackingService::updateStatus(int paketId, const std::string& status,
                                     const std::string& lokasi) {
    // Generate timestamp
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char timeBuf[30];
    sprintf(timeBuf, "%04d-%02d-%02d %02d:%02d:%02d",
            1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

    int newId = trackingHistory.size() + 1;
    Tracking tr(newId, paketId, lokasi, status, std::string(timeBuf));

    trackingHistory.push_back(tr);

    // UAS: Stack (3) - Push ke stack untuk undo
    undoStacks[paketId].push(tr);

    std::cout << "  [TRACKING] Paket ID " << paketId << " -> "
              << status << " di " << lokasi << "\n";
}

// ============================================================
// UNDO TRACKING
// UAS: Stack (3) - Pop dari stack untuk undone tracking
// ============================================================
bool TrackingService::undoLastTracking(int paketId) {
    auto it = undoStacks.find(paketId);
    if (it == undoStacks.end() || it->second.isEmpty()) {
        std::cout << "  [UNDO] Tidak ada tracking yang bisa di-undo untuk paket ID "
                  << paketId << ".\n";
        return false;
    }

    Tracking undone;
    if (it->second.pop(undone)) {
        // Hapus dari tracking history juga
        for (auto iter = trackingHistory.begin(); iter != trackingHistory.end(); ++iter) {
            if (iter->id == undone.id && iter->id_paket == paketId) {
                trackingHistory.erase(iter);
                break;
            }
        }
        std::cout << "  [UNDO] Tracking di-undo: " << undone.status
                  << " - " << undone.lokasi << "\n";
        return true;
    }
    return false;
}

// ============================================================
// CALLBACK FUNCTION
// UAS: callback function (5)
// ============================================================
void TrackingService::processTrackingHistory(
    int paketId,
    void (*callback)(const Tracking&)) {

    std::cout << "  [CALLBACK] Memproses tracking history untuk paket ID "
              << paketId << ":\n";
    for (const Tracking& tr : trackingHistory) {
        if (tr.id_paket == paketId) {
            callback(tr);  // UAS: callback function (5) - Panggil callback
        }
    }
}

// ============================================================
// DISPLAY
// ============================================================
void TrackingService::displayHistory(int paketId) const {
    std::cout << "\n  ===== TRACKING HISTORY PAKET ID " << paketId << " =====\n";
    bool found = false;
    for (const Tracking& tr : trackingHistory) {
        if (tr.id_paket == paketId) {
            std::cout << "  [" << tr.timestamp << "] "
                      << tr.status << " - " << tr.lokasi << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "  (Tidak ada tracking history)\n";
    }
}

void TrackingService::displayAllHistory() const {
    std::cout << "\n  ===== SEMUA TRACKING HISTORY =====\n";
    if (trackingHistory.empty()) {
        std::cout << "  (Tidak ada tracking history)\n";
        return;
    }
    for (const Tracking& tr : trackingHistory) {
        std::cout << "  [ID:" << tr.id << " | Paket:" << tr.id_paket
                  << "] " << tr.timestamp << " - " << tr.status
                  << " di " << tr.lokasi << "\n";
    }
}

void TrackingService::displayStack(int paketId) const {
    std::cout << "\n  ===== UNDO STACK PAKET ID " << paketId << " =====\n";
    auto it = undoStacks.find(paketId);
    if (it != undoStacks.end()) {
        it->second.display();
    } else {
        std::cout << "  (Stack kosong untuk paket ini)\n";
    }
}

// ============================================================
// GETTERS
// ============================================================
std::vector<Tracking> TrackingService::getHistory(int paketId) const {
    std::vector<Tracking> result;
    for (const Tracking& tr : trackingHistory) {
        if (tr.id_paket == paketId) {
            result.push_back(tr);
        }
    }
    return result;
}

std::vector<Tracking> TrackingService::getAllHistory() const {
    return trackingHistory;
}

Tracking TrackingService::getLatestStatus(int paketId) const {
    for (auto it = trackingHistory.rbegin(); it != trackingHistory.rend(); ++it) {
        if (it->id_paket == paketId) {
            return *it;
        }
    }
    // Return empty tracking jika tidak ditemukan
    return Tracking(0, paketId, "Tidak diketahui", "-", "-");
}

} // namespace SwiftExpedition