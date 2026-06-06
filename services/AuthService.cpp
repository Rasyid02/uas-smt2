#include "AuthService.h"
#include "../storage/CSVManager.h"
#include <iostream>
using namespace std;

namespace SwiftExpedition {

AuthService::AuthService() {
}

void AuthService::loadUsers(const string& filename) {

    try {
        vector<string> lines = CSVManager::read("data/" + filename);
        for (size_t i = 0; i < lines.size(); i++) {
            vector<string> tokens = CSVManager::split(lines[i], ';');

            if (tokens.size() >= 5) {
                int id = stoi(tokens[0]);
                string nama = tokens[1];
                string username = tokens[2];
                string password = tokens[3];
                RoleType role = stringToRole(tokens[4]);

                User user(id, nama, username, password, role);

                userTable.insert(username, user);

                cout << "  [LOAD] User: " << username
                          << " (" << roleToString(role) << ") berhasil dimuat.\n";
            }
        }
        cout << "  Total " << userTable.size() << " user dimuat dari " << filename << ".\n";
    }
    catch (const exception& e) {
        cout << "  [ERROR] Gagal memuat user: " << e.what() << "\n";
    }
}

User* AuthService::login(const string& username, const string& password) {

    User* user = userTable.find(username);
    if (user != nullptr && user->password == password) {
        return user;  
    }
    return nullptr;   
}

void AuthService::displayUsers() const {
    userTable.display();
}

bool AuthService::userExists(const string& username) const {
    return userTable.contains(username);
}

int AuthService::getUserCount() const {
    return userTable.size();
}

} 