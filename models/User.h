#pragma once
#include <string>
#include "Role.h"

// ============================================================
// UAS: struct (1) - Struct untuk data pengguna/login
// Menyimpan informasi akun untuk autentikasi multi-role
// ============================================================
struct User {
    int id;
    std::string nama;
    std::string username;
    std::string password;
    RoleType role;

    // UAS: default argument (6) - Konstruktor dengan nilai default
    User(int id = 0,
         const std::string& nama = "",
         const std::string& username = "",
         const std::string& password = "",
         RoleType role = RoleType::KURIR)
        : id(id), nama(nama), username(username), password(password), role(role) {}
};