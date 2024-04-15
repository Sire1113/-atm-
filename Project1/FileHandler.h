// FileHandler.h

#pragma once

#include "Account.h"
#include "EncryptionUtilities.h"
#include <iostream>
#include <fstream>
#include <sstream>

class FileHandler {
public:
    // д���˻���Ϣ���ļ�
    static bool writeAccountToFile(const Account& account, int key) {
        std::ofstream fileOut("accounts.dat", std::ios::app); // ʹ��׷��ģʽ
        if (!fileOut) {
            std::cerr << "�޷����ļ�д�롣\n";
            return false;
        }
        fileOut << EncryptionUtilities::encryptDecrypt(account.getID(), key)
            << account.getName() << " "
            << EncryptionUtilities::encryptDecrypt(account.getPassword(), key) << " "
            << account.getBalance() << std::endl;
        fileOut.close();
        return true;
    }

    // ���ļ��ж�ȡ�˻���Ϣ����֤����
    static bool verifyAccountPassword(const std::string& id, const std::string& password, int key) {
        std::ifstream fileIn("accounts.dat");
        if (!fileIn) {
            std::cerr << "�޷����ļ���ȡ��\n";
            return false;
        }

        std::string line;
        while (std::getline(fileIn, line)) {
            std::istringstream iss(line);
            std::string fileID, filePassword, fileName;
            double fileBalance;
            if (iss >> fileID >> fileName>> filePassword >> fileBalance) {
                if (fileID == id &&
                    EncryptionUtilities::encryptDecrypt(filePassword, -key) == password) {
                    fileIn.close();
                    return true;
                }
            }
        }
        fileIn.close();
        return false;
    }
    // �����˻�����
    static bool updatePassword(const std::string& id, const std::string& oldPassword, const std::string& newPassword, int key) {
        // ʹ��һ����ʱ�ļ��洢�µ��˻���Ϣ
        std::string tempFilename = "accounts_temp.dat";
        std::ifstream file("accounts.dat");
        std::ofstream tempFile(tempFilename);

        bool isUpdated = false;
        if (file.is_open() && tempFile.is_open()) {
            std::string fileID, fileName, filePassword;
            double fileBalance;

            while (file >> fileID >> fileName >> filePassword >> fileBalance) {
                if (fileID == EncryptionUtilities::encryptDecrypt(id, -key)) {
                    // ���ܲ��Ƚ�����
                    std::string decryptedPassword = EncryptionUtilities::encryptDecrypt(filePassword, -key);
                    if (decryptedPassword == oldPassword) {
                        // ����������ȷ��ʹ�������벢���ܺ�д����ʱ�ļ�
                        isUpdated = true;
                        filePassword = EncryptionUtilities::encryptDecrypt(newPassword, key);
                    }
                    else {
                        // �����벻��ȷ����������Ϣ������ʧ��
                        std::cerr << "�����벻��ȷ���޷����¡�\n";
                        return false;
                    }
                }
                // ���˻���Ϣд����ʱ�ļ���
                tempFile << fileID << " " << fileName << " " << filePassword << " " << fileBalance << "\n";
            }

            file.close();
            tempFile.close();

            if (isUpdated) {
                // ʹ���µ��˻��ļ��滻�ɵ��ļ�
                remove("accounts.dat");
                rename(tempFilename.c_str(), "accounts.dat");
            }
            else {
                // ��û�и����κ����ݣ�ɾ����ʱ�ļ�
                remove(tempFilename.c_str());
                std::cerr << "û���ҵ���Ӧ���˻����и��¡�\n";
            }

        }
        else {
            std::cerr << "�޷����ļ����и��¡�\n";
            return false;
        }
        return isUpdated;
    }
};


