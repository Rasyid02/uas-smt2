#pragma once
#include "../models/User.h"
using namespace std;

namespace SwiftExpedition {
class PaketService;
class AdminMenu {
public:

    bool show(User* user, PaketService& paket);
};
}