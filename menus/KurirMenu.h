#pragma once
#include "../models/User.h"
using namespace std;

namespace SwiftExpedition {

class PaketService;
class TrackingService;

class KurirMenu {
public:

    bool show(User* user, PaketService& paket, TrackingService& tracking);
};

} 