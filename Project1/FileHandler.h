// FileHandler.h

#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include "Account.h"
#include "VigenereCipher.h"

class FileHandler {
private:
    // ����һ���̶�����Կ
    std::string key = "VigenereKey"; 

public:
    // ���˻���Ϣд���ļ�
    static void writeAccountToFile(const Account& account) {
        VigenereCipher cipher;

        std::ofstream file("accounts.dat", std::ios::app); // ׷��ģʽ
        if (file.is_open()) {
            file << account.getID() << " ";
            file << account.getName() << " ";
            // ʹ��VigenereCipher�������벢д���ļ�
            file << cipher.encrypt(account.getPassword(), "VigenereKey") << " ";
            file << account.getBalance() << "\n";
            file.close();
        }
        else {
            std::cerr << "�޷����ļ�д���˻���Ϣ��\n";
        }
    }

    // ���ļ��ж�ȡ�˻���Ϣ����֤����
    static bool verifyAccountLogin(const std::string& id, const std::string& password) {
        VigenereCipher cipher;

        std::ifstream file("accounts.dat");
        if (file.is_open()) {
            std::string fileID, fileName, fileEncryptedPassword;
            double fileBalance;

            while (file >> fileID >> fileName >> fileEncryptedPassword >> fileBalance) {
                // ����ҵ�ƥ���ID�����Խ��ܲ���֤����
                if (fileID == id) {
                    // ʹ��VigenereCipher�Ľ��ܺ��������ļ��е�����
                    std::string decryptedPassword = cipher.decrypt(fileEncryptedPassword,"VigenereKey");
                    std::cout << decryptedPassword << std::endl ;
                    // �ȽϽ��ܺ���������û����������Ƿ���ͬ
                    return decryptedPassword == password;
                }
            }
            file.close();
            std::cerr << "�˻�IDδ�ҵ���\n";
        }
        else {
            std::cerr << "�޷����ļ�������֤��\n";
        }
        return false;
    }
};