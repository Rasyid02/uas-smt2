#pragma once
#include "../models/User.h"
#include <vector>
#include "../models/Paket.h"
using namespace std;

namespace SwiftExpedition {

class PaketService;
class ReportService;
class TrackingService;

class ManagerMenu {
public:

    bool show(User* user, PaketService& paket, ReportService& report, TrackingService& tracking);
};

} 