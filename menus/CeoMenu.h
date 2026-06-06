#pragma once
#include "../models/User.h"
using namespace std;

namespace SwiftExpedition {

class PaketService;
class ReportService;
class TrackingService;
class RoutingService;

class CeoMenu {
public:

    bool show(User* user, PaketService& paket, ReportService& report, 
              TrackingService& tracking, RoutingService& routing);
};

} 