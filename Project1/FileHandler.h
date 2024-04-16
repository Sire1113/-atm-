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
        fileOut << EncryptionUtilities::encryptDecrypt(account.getID(), key) << " "
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
                if (fileID == EncryptionUtilities::encryptDecrypt(id, -key) &&
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
    // д�뽻�׼�¼���ļ�
    static bool writeStatementToFile(const Statement& statement) {
        std::ofstream fileOut("statements.dat", std::ios::app); // ʹ��׷��ģʽ�򿪽�����ˮ��¼�ļ�
        if (!fileOut) {
            std::cerr << "�޷����ļ�д�뽻�׼�¼��\n";
            return false;
        }

        // ���뽻����Ϣ����ʽ��Ϊ��ˮ��� �˻����� ����ʱ�� �������� ���׽�� �Է��˻�
        fileOut << statement.getID() << " "
            << statement.getAccountID() << " "
            << statement.getTime() << " "
            << statement.getType() << " "
            << statement.getAmount() << " "
            << statement.getToAccountID() << std::endl;
        fileOut.close();
        return true;
    }

    // ���ļ��ж�ȡ�˻������н�����ˮ��¼
    static void readStatements(const std::string& accountID) {
        std::ifstream fileIn("statements.dat");
        if (!fileIn) {
            std::cerr << "�޷����ļ���ȡ���׼�¼��\n";
            return;
        }

        std::string line;
        std::cout << "��ˮ��� �˻����� ����ʱ�� �������� ���׽�� �Է��˻�\n";
        while (getline(fileIn, line)) {
            
            std::istringstream iss(line);
            std::string id, accID, time, toAccID;
            int type;
            double amount;
            if (iss >> id >> accID >> time >> type >> amount >> toAccID) {
                //std::cout << 1 << std::endl;
                if (accID == accountID) {
                    std::cout << id << " " << accID << " " << time << " " << type << " " << amount << " " << toAccID << std::endl;
                }
                else {
                    //std::cout << accID << std::endl;
                }
            }
        }
        fileIn.close();
    }

    // �����˻����
    static bool updateAccountBalance(const std::string& id, double newBalance, int key) {
        std::ifstream fileIn("accounts.dat");
        std::ofstream fileOut("accounts_temp.dat");
        bool isUpdated = false;

        if (fileIn && fileOut) {
            std::string fileID, fileName, filePassword;
            double fileBalance;
            while (fileIn >> fileID >> fileName >> filePassword >> fileBalance) {
                if (fileID == EncryptionUtilities::encryptDecrypt(id, -key)) {
                    fileBalance = newBalance;  // �������
                    isUpdated = true;
                }
                fileOut << fileID
                    << " " << fileName << " "
                    << filePassword << " "
                    << fileBalance << std::endl;
            }
            fileIn.close();
            fileOut.close();

            // ɾ�����˻��ļ�������ʱ�ļ�������Ϊ���˻��ļ�
            remove("accounts.dat");
            rename("accounts_temp.dat", "accounts.dat");
        }
        else {
            std::cerr << "�޷����ļ����и��¡�\n";
            if (fileIn) fileIn.close();
            if (fileOut) {
                fileOut.close();
                remove("accounts_temp.dat");  // ɾ����ʱ�ļ�
            }
            return false;
        }
        return isUpdated;
    }

    // ��ȡ�˻����
    static double getAccountBalance(const std::string& id, int key) {
        std::ifstream fileIn("accounts.dat");
        if (!fileIn) {
            std::cerr << "�޷����ļ�����ȡ��\n";
            return -1; // ʹ�ô���ֵ��ʾ�޷���ȡ���
        }

        std::string fileID, fileName, filePassword;
        double fileBalance;
        while (fileIn >> fileID >> fileName >> filePassword >> fileBalance) {
            if (fileID == EncryptionUtilities::encryptDecrypt(id, -key)) {
                fileIn.close();
                return fileBalance; // ���ؽ��ܺ���˻����
            }
        }
        fileIn.close();
        std::cerr << "û�ҵ���Ӧ���˻���\n";
        return -1; // ʹ�ô���ֵ��ʾ�˻�������
    }
    
    // ��ID�����˻���Ϣ
    static Account* retrieveAccountById(const std::string& id, int key) {
        std::ifstream fileIn("accounts.dat");
        if (!fileIn) {
            std::cerr << "�޷����ļ���ȡ�˻���Ϣ��\n";
            return nullptr; // ���� nullptr ��ʾû�ҵ��˻�
        }

        std::string fileID, fileName, filePassword;
        double fileBalance;
        while (fileIn >> fileID >> fileName >> filePassword >> fileBalance) {
            // ���ܲ����ID�Ƿ�ƥ��
            if (EncryptionUtilities::encryptDecrypt(fileID, -key) == id) {
                fileIn.close();
                // ���ض�̬����Ķ�Ӧ���˻�����
                return new Account(id, fileName, filePassword, fileBalance);
            }
        }

        fileIn.close();
        std::cerr << "δ�ҵ�ƥ����˻�ID��\n";
        return nullptr; // δ�ҵ��˻������� nullptr
    }
    /*
    //ת��
    void transfer(Account& fromAccount, Account& toAccount, double amount, int key) {
        // ȷ��֧����������Ƿ��㹻
        double fromBalance = FileHandler::getAccountBalance(fromAccount.getID(), key);
        if (fromBalance < amount) {
            std::cout << "������㣬ת��ʧ�ܡ�\n";
            return;
        }

        // ���˻��ļ��м��ٸ�����������տ�����
        if (fromAccount.withdraw(amount)) {
            FileHandler::updateAccountBalance(fromAccount.getID(), fromAccount.getBalance(), key);
            if (toAccount.deposit(amount)) {
                FileHandler::updateAccountBalance(toAccount.getID(), toAccount.getBalance(), key);
                std::cout << "ת�˳ɹ���\n";
            }
            else {
                // ����տ�ʧ�ܣ���Ҫ�ع�����Ŀۿ�
                fromAccount.deposit(amount);
                FileHandler::updateAccountBalance(fromAccount.getID(), fromAccount.getBalance(), key);
                std::cout << "ת��ʧ�ܣ��տ�˻����⡣\n";
            }
        }
        else {
            std::cout << "ת��ʧ�ܣ��ۿ�δ�ɹ���\n";
        }

        // ��¼���׵���ˮ��¼�ļ�
        // ... ��¼�����֧��
        // ... ��¼�տ������
    }
    */
};


