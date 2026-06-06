#include "AuthService.h"
#include "../storage/CSVManager.h"
#include <iostream>

// ============================================================
// UAS: AuthService - Implementasi autentikasi
// ============================================================

namespace SwiftExpedition {

AuthService::AuthService() {
}

void AuthService::loadUsers(const std::string& filename) {
    // UAS: file handling (15) - Membaca CSV
    // UAS: exception handling (9)
    try {
        std::vector<std::string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            std::vector<std::string> tokens = CSVManager::split(lines[i], ';');
            // CSV: id;nama;username;password;role
            if (tokens.size() >= 5) {
                int id = std::stoi(tokens[0]);
                std::string nama = tokens[1];
                std::string username = tokens[2];
                std::string password = tokens[3];
                RoleType role = stringToRole(tokens[4]);

                User user(id, nama, username, password, role);
                // UAS: HashTable (9) - Insert dengan key username
                userTable.insert(username, user);

                std::cout << "  [LOAD] User: " << username
                          << " (" << roleToString(role) << ") berhasil dimuat.\n";
            }
        }
        std::cout << "  Total " << userTable.size() << " user dimuat dari " << filename << ".\n";
    }
    catch (const std::exception& e) {
        std::cout << "  [ERROR] Gagal memuat user: " << e.what() << "\n";
    }
}

User* AuthService::login(const std::string& username, const std::string& password) {
    // UAS: pointer (*) - 2: Return pointer ke User
    User* user = userTable.find(username);
    if (user != nullptr && user->password == password) {
        return user;  // Login berhasil
    }
    return nullptr;   // Login gagal
}

void AuthService::displayUsers() const {
    userTable.display();
}

bool AuthService::userExists(const std::string& username) const {
    return userTable.contains(username);
}

int AuthService::getUserCount() const {
    return userTable.size();
}

} // namespace SwiftExpedition