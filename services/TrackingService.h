#pragma once
#include <string>
#include <vector>
#include <map>
#include "../models/Tracking.h"
#include "../datastructures/Stack.h"

// ============================================================
// UAS: TrackingService - Manajemen tracking pengiriman
// Menggunakan Stack (Struktur Data 3) untuk UNDO STATUS
// UAS: callback function (5)
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

class TrackingService {
private:
    std::vector<Tracking> trackingHistory;

public:
    TrackingService();

    // ========== LOAD & SAVE ==========
    // UAS: file handling (15)
    void loadTracking(const std::string& filename);
    void saveTracking(const std::string& filename);

    // ========== UPDATE TRACKING ==========
    // Setiap update disimpan ke tracking vector dan stack (untuk undo)
    void updateStatus(int paketId, const std::string& status, const std::string& lokasi);

    // ========== STACK UNTUK UNDO ==========
    // UAS: Stack (3) - Undo status tracking
    bool undoLastTracking(int paketId);

    // ========== CALLBACK FUNCTION ==========
    // UAS: callback function (5) - Memproses setiap tracking entry
    void processTrackingHistory(
        int paketId,
        void (*callback)(const Tracking&)  // UAS: callback function (5)
    );

    // ========== DISPLAY ==========
    void displayHistory(int paketId) const;
    void displayAllHistory() const;
    void displayStack(int paketId) const;

    // ========== GETTERS ==========
    std::vector<Tracking> getHistory(int paketId) const;
    std::vector<Tracking> getAllHistory() const;
    Tracking getLatestStatus(int paketId) const;

private:
    // Stack per paket untuk undo (key: paketId)
    std::map<int, Stack<Tracking> > undoStacks;
};

} // namespace SwiftExpedition