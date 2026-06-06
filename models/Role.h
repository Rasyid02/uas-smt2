#pragma once
#include <string>

// ============================================================
// UAS: struct (1) - Penggunaan struct untuk enumerasi role
// Digunakan untuk menentukan hak akses pengguna dalam sistem
// ============================================================
enum class RoleType {
    CEO,
    MANAGER,
    ADMIN,
    KURIR
};

// UAS: inline function (7) - Konversi RoleType ke string untuk display
inline std::string roleToString(RoleType r) {
    switch (r) {
        case RoleType::CEO:     return "CEO";
        case RoleType::MANAGER: return "Manager";
        case RoleType::ADMIN:   return "Admin";
        case RoleType::KURIR:   return "Kurir";
        default:                return "Unknown";
    }
}

// UAS: inline function (7) - Konversi string ke RoleType untuk parsing CSV
inline RoleType stringToRole(const std::string& s) {
    if (s == "CEO" || s == "ceo")     return RoleType::CEO;
    if (s == "Manager" || s == "manager") return RoleType::MANAGER;
    if (s == "Admin" || s == "admin") return RoleType::ADMIN;
    if (s == "Kurir" || s == "kurir") return RoleType::KURIR;
    return RoleType::KURIR;
}