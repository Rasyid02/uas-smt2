#pragma once
#include <string>
using namespace std;

enum class RoleType {
    CEO,
    MANAGER,
    ADMIN,
    KURIR
};

inline string roleToString(RoleType r) {
    switch (r) {
        case RoleType::CEO:     return "CEO";
        case RoleType::MANAGER: return "Manager";
        case RoleType::ADMIN:   return "Admin";
        case RoleType::KURIR:   return "Kurir";
        default:                return "Unknown";
    }
}

inline RoleType stringToRole(const string& s) {
    if (s == "CEO" || s == "ceo")     return RoleType::CEO;
    if (s == "Manager" || s == "manager") return RoleType::MANAGER;
    if (s == "Admin" || s == "admin") return RoleType::ADMIN;
    if (s == "Kurir" || s == "kurir") return RoleType::KURIR;
    return RoleType::KURIR;
}