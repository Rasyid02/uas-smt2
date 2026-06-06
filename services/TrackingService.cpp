#include "TrackingService.h"
#include "../storage/CSVManager.h"
#include <iostream>
#include <ctime>
using namespace std;

namespace SwiftExpedition {

TrackingService::TrackingService() {
}

void TrackingService::loadTracking(const string& filename) {
    try {
        vector<string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            vector<string> t = CSVManager::split(lines[i], ';');

            if (t.size() >= 5) {
                Tracking tr(
                    stoi(t[0]),
                    stoi(t[1]),
                    t[2],   
                    t[3],   
                    t[4]    
                );
                trackingHistory.push_back(tr);

                undoStacks[tr.id_paket].push(tr);
            }
        }
        cout << "  [LOAD] " << trackingHistory.size()
                  << " tracking history dimuat dari " << filename << ".\n";
    }
    catch (const exception& e) {
        cout << "  [WARNING] Gagal memuat tracking: " << e.what() << "\n";
    }
}

void TrackingService::saveTracking(const string& filename) {
    vector<string> lines;
    lines.push_back("id;id_paket;lokasi;status;timestamp");
    for (const Tracking& tr : trackingHistory) {
        vector<string> row = {
            to_string(tr.id),
            to_string(tr.id_paket),
            tr.lokasi,
            tr.status,
            tr.timestamp
        };
        lines.push_back(CSVManager::join(row, ';'));
    }
    CSVManager::write("data/" + filename, lines);
    cout << "  [SUKSES] " << trackingHistory.size()
              << " tracking history disimpan ke " << filename << ".\n";
}

void TrackingService::updateStatus(int paketId, const string& status,
                                     const string& lokasi) {

    time_t now = time(0);
    tm* ltm = localtime(&now);
    char timeBuf[30];
    sprintf(timeBuf, "%04d-%02d-%02d %02d:%02d:%02d",
            1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

    int newId = trackingHistory.size() + 1;
    Tracking tr(newId, paketId, lokasi, status, string(timeBuf));

    trackingHistory.push_back(tr);

    undoStacks[paketId].push(tr);

    cout << "  [TRACKING] Paket ID " << paketId << " -> "
              << status << " di " << lokasi << "\n";
}

bool TrackingService::undoLastTracking(int paketId) {
    auto it = undoStacks.find(paketId);
    if (it == undoStacks.end() || it->second.isEmpty()) {
        cout << "  [UNDO] Tidak ada tracking yang bisa di-undo untuk paket ID "
                  << paketId << ".\n";
        return false;
    }

    Tracking undone;
    if (it->second.pop(undone)) {

        for (auto iter = trackingHistory.begin(); iter != trackingHistory.end(); ++iter) {
            if (iter->id == undone.id && iter->id_paket == paketId) {
                trackingHistory.erase(iter);
                break;
            }
        }
        cout << "  [UNDO] Tracking di-undo: " << undone.status
                  << " - " << undone.lokasi << "\n";
        return true;
    }
    return false;
}

void TrackingService::processTrackingHistory(
    int paketId,
    void (*callback)(const Tracking&)) {

    cout << "  [CALLBACK] Memproses tracking history untuk paket ID "
              << paketId << ":\n";
    for (const Tracking& tr : trackingHistory) {
        if (tr.id_paket == paketId) {
            callback(tr);  
        }
    }
}

void TrackingService::displayHistory(int paketId) const {
    cout << "\n  ===== TRACKING HISTORY PAKET ID " << paketId << " =====\n";
    bool found = false;
    for (const Tracking& tr : trackingHistory) {
        if (tr.id_paket == paketId) {
            cout << "  [" << tr.timestamp << "] "
                      << tr.status << " - " << tr.lokasi << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "  (Tidak ada tracking history)\n";
    }
}

void TrackingService::displayAllHistory() const {
    cout << "\n  ===== SEMUA TRACKING HISTORY =====\n";
    if (trackingHistory.empty()) {
        cout << "  (Tidak ada tracking history)\n";
        return;
    }
    for (const Tracking& tr : trackingHistory) {
        cout << "  [ID:" << tr.id << " | Paket:" << tr.id_paket
                  << "] " << tr.timestamp << " - " << tr.status
                  << " di " << tr.lokasi << "\n";
    }
}

void TrackingService::displayStack(int paketId) const {
    cout << "\n  ===== UNDO STACK PAKET ID " << paketId << " =====\n";
    auto it = undoStacks.find(paketId);
    if (it != undoStacks.end()) {
        it->second.display();
    } else {
        cout << "  (Stack kosong untuk paket ini)\n";
    }
}

vector<Tracking> TrackingService::getHistory(int paketId) const {
    vector<Tracking> result;
    for (const Tracking& tr : trackingHistory) {
        if (tr.id_paket == paketId) {
            result.push_back(tr);
        }
    }
    return result;
}

vector<Tracking> TrackingService::getAllHistory() const {
    return trackingHistory;
}

Tracking TrackingService::getLatestStatus(int paketId) const {
    for (auto it = trackingHistory.rbegin(); it != trackingHistory.rend(); ++it) {
        if (it->id_paket == paketId) {
            return *it;
        }
    }

    return Tracking(0, paketId, "Tidak diketahui", "-", "-");
}

} 