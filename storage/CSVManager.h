#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

namespace SwiftExpedition {  

class CSVManager {
public:

    static vector<string> read(const string& filename, bool skipHeader = true) {
        vector<string> lines;
        ifstream file(filename);

        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file: " + filename);
        }

        string line;
        bool isFirstLine = true;
        while (getline(file, line)) {
            if (skipHeader && isFirstLine) {
                isFirstLine = false;
                continue;  
            }
            isFirstLine = false;
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        file.close();
        return lines;
    }

    static void write(const string& filename, const vector<string>& lines) {
        ofstream file(filename);

        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file untuk ditulis: " + filename);
        }

        for (const string& line : lines) {
            file << line << "\n";
        }
        file.close();
    }

    static void append(const string& filename, const string& line) {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file untuk append: " + filename);
        }
        file << line << "\n";
        file.close();
    }

    static vector<string> split(const string& line, char delimiter = ',') {
        vector<string> tokens;
        stringstream ss(line);
        string token;

        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    static string join(const vector<string>& tokens, char delimiter = ',') {
        string result;
        for (size_t i = 0; i < tokens.size(); i++) {
            result += tokens[i];
            if (i < tokens.size() - 1) result += delimiter;
        }
        return result;
    }
};

} 