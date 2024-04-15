// EncryptionUtilities.h

#pragma once

#include <string>

class EncryptionUtilities {
public:
    // �򵥵����ֶԳƼ��ܺ������������ڴ������ַ���
    static std::string encryptDecrypt(const std::string& input, int key) {
        std::string result = input;

        for (char& c : result) {
            if (isdigit(c)) { // �����������ַ�
                // '0' - '9' ��ASCIIֵ�� 48 - 57
                // ���ַ�ת��Ϊ0 - 9�����м��ܲ�������ת�����ַ�
                c = (c - '0' + key + 10) % 10 + '0';
            }
        }

        return result;
    }
};