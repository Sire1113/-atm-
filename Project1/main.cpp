// main.cpp
#include <windows.h>
#include "Account.h"
#include <fstream>
#include <iostream>
#include "FileHandler.h"


#include <iostream>
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

    Account newAccount(id, name, password, balance);
    FileHandler::writeAccountToFile(newAccount);
    std::cout << "�˻������ɹ���\n";
}

// ��֤��¼�ĺ���
void verifyLogin() {
    std::string id, password;

    std::cout << "�����뿨��: ";
    std::cin >> id;
    std::cout << "����������: ";
    std::cin >> password;

    if (FileHandler::verifyAccountLogin(id, password)) {
        std::cout << "������֤�ɹ���\n";
    }
    else {
        std::cout << "������֤ʧ�ܣ�\n";
    }
}
/*
int main() {
    int choice;

    std::cout << "��ѡ�������\n";
    std::cout << "1. �����˻�\n";
    std::cout << "2. ��֤��¼\n";
    std::cin >> choice;

    switch (choice) {
    case 1:
        createAccount();
        break;
    case 2:
        verifyLogin();
        break;
    default:
        std::cout << "��Чѡ�\n";
    }

    return 0;
}

*/
int main() {
    VigenereCipher cipher;

    // ʾ�����ĺ���Կ
    std::string plaintext = "Hello123World";
    std::string key = "VIGENERECIPHERKEY";

    std::cout << "Original text : " << plaintext << "\n";
    std::cout << "Key           : " << key << "\n";

    // ����
    std::string encryptedText = cipher.encrypt(plaintext, key);
    std::cout << "Encrypted text: " << encryptedText << "\n";

    // ����
    std::string decryptedText = cipher.decrypt(encryptedText, key);
    std::cout << "Decrypted text: " << decryptedText << "\n";

    return 0;
}