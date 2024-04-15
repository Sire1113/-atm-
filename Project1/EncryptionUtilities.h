// EncryptionUtilities.h

#pragma once

#include <string>

class EncryptionUtilities {
public:
    // �򵥵Ŀ������ܺ���
    static std::string caesarEncrypt(const std::string& input, int shift) {
        std::string encrypted = input;

        for (char& c : encrypted) {
            if (isalpha(c)) {
                char base = isupper(c) ? 'A' : 'a';
                c = static_cast<char>((c - base + shift) % 26 + base);
            }
        }

        return encrypted;
    }
};