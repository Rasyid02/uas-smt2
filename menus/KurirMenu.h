#pragma once
#include "../models/User.h"

namespace SwiftExpedition {

class PaketService;
class TrackingService;

class KurirMenu {
public:
    // Returns true if user chose logout, false if exit program
    bool show(User* user, PaketService& paket, TrackingService& tracking);
};

} // namespace SwiftExpedition