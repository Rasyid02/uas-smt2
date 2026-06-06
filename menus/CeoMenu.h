#pragma once
#include "../models/User.h"

namespace SwiftExpedition {

class PaketService;
class ReportService;
class TrackingService;
class RoutingService;

class CeoMenu {
public:
    // Returns true if user chose logout, false if exit program
    bool show(User* user, PaketService& paket, ReportService& report, 
              TrackingService& tracking, RoutingService& routing);
};

} // namespace SwiftExpedition