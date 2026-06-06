#pragma once
#include "../models/User.h"
#include <vector>
#include "../models/Paket.h"

namespace SwiftExpedition {

class PaketService;
class ReportService;
class TrackingService;

class ManagerMenu {
public:
    // Returns true if user chose logout, false if exit program
    bool show(User* user, PaketService& paket, ReportService& report, TrackingService& tracking);
};

} // namespace SwiftExpedition