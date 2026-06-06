#pragma once
#include <string>
#include <vector>
#include <map>
#include "../models/Tracking.h"
#include "../datastructures/Stack.h"
using namespace std;

namespace SwiftExpedition {  

class TrackingService {
private:
    vector<Tracking> trackingHistory;

public:
    TrackingService();

    void loadTracking(const string& filename);
    void saveTracking(const string& filename);

    void updateStatus(int paketId, const string& status, const string& lokasi);

    bool undoLastTracking(int paketId);

    void processTrackingHistory(
        int paketId,
        void (*callback)(const Tracking&)  
    );

    void displayHistory(int paketId) const;
    void displayAllHistory() const;
    void displayStack(int paketId) const;

    vector<Tracking> getHistory(int paketId) const;
    vector<Tracking> getAllHistory() const;
    Tracking getLatestStatus(int paketId) const;

private:

    map<int, Stack<Tracking> > undoStacks;
};

} 