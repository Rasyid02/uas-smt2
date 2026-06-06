#pragma once
#include "../models/User.h"

namespace SwiftExpedition {
class PaketService;
class AdminMenu {
public:
    // Returns true if user chose logout, false if exit program
    bool show(User* user, PaketService& paket);
};
}