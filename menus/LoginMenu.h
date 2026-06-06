#pragma once
#include "../models/User.h"
using namespace std;

namespace SwiftExpedition {

class AuthService;

class LoginMenu {
public:

    User* show(AuthService& auth);
};

} 