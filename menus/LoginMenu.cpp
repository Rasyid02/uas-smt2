#include "LoginMenu.h"
#include "../services/AuthService.h"
#include <iostream>
#include <limits>
#include <string>

namespace SwiftExpedition {

User* LoginMenu::show(AuthService& auth) {
    std::string username, password;
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        std::cout << "\n  ============================================\n";
        std::cout << "  === SWIFT EXPEDITION - LOGIN ===\n";
        std::cout << "  ============================================\n";
        std::cout << "  Username : ";
        std::getline(std::cin, username);
        std::cout << "  Password : ";
        std::getline(std::cin, password);

        if (username.empty() || password.empty()) {
            std::cout << "\n  [ERROR] Username dan password tidak boleh kosong!\n";
            attempts++;
            continue;
        }

        User* user = auth.login(username, password);

        if (user != nullptr) {
            std::cout << "\n  ============================================\n";
            std::cout << "  Selamat datang, " << user->nama << "!\n";
            std::cout << "  Role: " << roleToString(user->role) << "\n";
            std::cout << "  ============================================\n";
            return user;
        }

        attempts++;
        std::cout << "\n  [ERROR] Login gagal! Sisa percobaan: "
                  << (maxAttempts - attempts) << "\n";
    }

    std::cout << "\n  [ERROR] Terlalu banyak percobaan login. Program berhenti.\n";
    return nullptr;
}

} // namespace SwiftExpedition