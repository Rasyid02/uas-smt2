#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// ============================================================
// UAS: File Handling (15) - ifstream / ofstream
// Digunakan untuk membaca dan menulis file CSV
// ============================================================

namespace SwiftExpedition {  // UAS: namespace (4)

class CSVManager {
public:
    // UAS: file handling (15) - Membaca file CSV, return vector of string (per baris)
    // UAS: exception handling (9) - try/catch untuk file tidak ditemukan
    // UAS: default argument (6) - skipHeader=true untuk melewati baris header
    // UAS: function overloading (8) - Overload read dengan opsi skip header
    static std::vector<std::string> read(const std::string& filename, bool skipHeader = true) {
        std::vector<std::string> lines;
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Gagal membuka file: " + filename);
        }

        std::string line;
        bool isFirstLine = true;
        while (std::getline(file, line)) {
            if (skipHeader && isFirstLine) {
                isFirstLine = false;
                continue;  // Skip baris header CSV
            }
            isFirstLine = false;
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        file.close();
        return lines;
    }

    // UAS: file handling (15) - Menulis file CSV (overwrite, bukan append)
    // UAS: exception handling (9)
    static void write(const std::string& filename, const std::vector<std::string>& lines) {
        std::ofstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Gagal membuka file untuk ditulis: " + filename);
        }

        for (const std::string& line : lines) {
            file << line << "\n";
        }
        file.close();
    }

    // UAS: function overloading (8) - Append satu baris ke file CSV
    static void append(const std::string& filename, const std::string& line) {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            throw std::runtime_error("Gagal membuka file untuk append: " + filename);
        }
        file << line << "\n";
        file.close();
    }

    // Memisahkan string CSV menjadi vector token
    // UAS: reference (&) - 3
    static std::vector<std::string> split(const std::string& line, char delimiter = ',') {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Menggabungkan vector token menjadi string CSV
    static std::string join(const std::vector<std::string>& tokens, char delimiter = ',') {
        std::string result;
        for (size_t i = 0; i < tokens.size(); i++) {
            result += tokens[i];
            if (i < tokens.size() - 1) result += delimiter;
        }
        return result;
    }
};

} // namespace SwiftExpedition