// main.cpp
#include <windows.h>
#include "Account.h"
#include <fstream>
#include <iostream>
#include "FileHandler.h"
#include <limits>
#include <string>
#include <cctype>

// �����˻��ĺ���
void createAccount() {
    std::string id, name, password;
    double balance;

    std::cout << "�����뿨��: ";
    std::cin >> id;
    std::cout << "����������: ";
    std::cin >> name;
    std::cout << "����������: ";
    std::cin >> password;
    std::cout << "�������˻���ʼ���: ";
    std::cin >> balance;

    // �����˻�ʵ��
    Account newAccount(id, name, password, balance);

    // д���ļ�
    if (FileHandler::writeAccountToFile(newAccount, 5)) {
        std::cout << "�˻������ɹ���\n";
    }
    else {
        std::cout << "�˻�����ʧ�ܣ����Ժ����ԡ�\n";
    }
}

// ��֤��¼�ĺ���
// ��֤��¼�ĺ���
void verifyLogin() {
    std::string id, password;

    std::cout << "�����뿨��: ";
    std::cin >> id;
    std::cout << "����������: ";
    std::cin >> password;

    if (FileHandler::verifyAccountPassword(id, password, 5)) {
        std::cout << "������֤�ɹ�����ӭ��½��\n";
    }
    else {
        std::cout << "������֤ʧ�ܣ����Ż��������\n";
    }
}


int main() {
    while (1)
    {
        std::cout << "ATMϵͳ\n";
        std::cout << "0. �˳�\n";
        std::cout << "1. �����˻�\n";
        std::cout << "2. ��֤����\n";
        std::cout << "3. �޸�����\n";
        std::cout << "��ѡ��һ��ѡ��: ";

        int option;
        std::cin >> option;

        switch (option) {
        case 1:
            createAccount();
            break;
        case 2:
            verifyLogin();
            break;
        case 3: {
            std::string id = "123";
            int key = 5;
            std::string oldPassword, newPassword;
            std::cout << "oldpassword:";
            std::cin >> oldPassword;
            std::cout << "newpassword:";
            std::cin >> newPassword;
            bool passwordChanged = FileHandler::updatePassword(id, oldPassword, newPassword, key);
            if (passwordChanged) {
                std::cout << "������³ɹ���" << std::endl;
            }
            else {
                std::cout << "�������ʧ�ܡ�" << std::endl;
            }
        }
        case 0:
            return 0;
        default:
            std::cout << "��Чѡ�������ѡ��\n";
            break;
        }
    }
    
}