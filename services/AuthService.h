#pragma once
#include <string>
#include "../models/User.h"
#include "../datastructures/HashTable.h"
using namespace std;

namespace SwiftExpedition {  

class AuthService {
private:

    HashTable<string, User> userTable;

public:
    AuthService();

    void loadUsers(const string& filename);

    User* login(const string& username, const string& password);

    void displayUsers() const;

    bool userExists(const string& username) const;

    int getUserCount() const;
};

} 