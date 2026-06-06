#include "LoginMenu.h"
#include "../services/AuthService.h"
#include <iostream>
#include <limits>
#include <string>
using namespace std;

namespace SwiftExpedition {

User* LoginMenu::show(AuthService& auth) {
    string username, password;
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        cout << "\n  ============================================\n";
        cout << "  === SWIFT EXPEDITION - LOGIN ===\n";
        cout << "  ============================================\n";
        cout << "  Username : ";
        getline(cin, username);
        cout << "  Password : ";
        getline(cin, password);

        if (username.empty() || password.empty()) {
            cout << "\n  [ERROR] Username dan password tidak boleh kosong!\n";
            attempts++;
            continue;
        }

        User* user = auth.login(username, password);

        if (user != nullptr) {
            cout << "\n  ============================================\n";
            cout << "  Selamat datang, " << user->nama << "!\n";
            cout << "  Role: " << roleToString(user->role) << "\n";
            cout << "  ============================================\n";
            return user;
        }

        attempts++;
        cout << "\n  [ERROR] Login gagal! Sisa percobaan: "
                  << (maxAttempts - attempts) << "\n";
    }

    cout << "\n  [ERROR] Terlalu banyak percobaan login. Program berhenti.\n";
    return nullptr;
}

} 