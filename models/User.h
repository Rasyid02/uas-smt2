#pragma once
#include <string>
#include "Role.h"
using namespace std;

struct User {
    int id;
    string nama;
    string username;
    string password;
    RoleType role;

    User(int id = 0,
         const string& nama = "",
         const string& username = "",
         const string& password = "",
         RoleType role = RoleType::KURIR)
        : id(id), nama(nama), username(username), password(password), role(role) {}
};