#pragma once
#include "../models/User.h"

// ============================================================
// UAS: LoginMenu - Menu login multi-role
// ============================================================
namespace SwiftExpedition {

class AuthService;

class LoginMenu {
public:
    // Returns logged-in user (nullptr if failed)
    User* show(AuthService& auth);
};

} // namespace SwiftExpedition