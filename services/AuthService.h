#pragma once
#include <string>
#include "../models/User.h"
#include "../datastructures/HashTable.h"

// ============================================================
// UAS: AuthService - Layanan autentikasi multi-role
// Menggunakan HashTable untuk lookup O(1)
// UAS: exception handling (9)
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

class AuthService {
private:
    // UAS: HashTable (9) untuk menyimpan user dengan key username
    HashTable<std::string, User> userTable;

public:
    AuthService();

    // Load user dari file CSV anggota.csv
    void loadUsers(const std::string& filename);

    // UAS: pointer (*) - 2: Login, return pointer ke User yang login
    // UAS: exception handling (9)
    User* login(const std::string& username, const std::string& password);

    // Menampilkan semua user
    void displayUsers() const;

    // Mengecek apakah user ada
    bool userExists(const std::string& username) const;

    // Mendapatkan jumlah user
    int getUserCount() const;
};

} // namespace SwiftExpedition