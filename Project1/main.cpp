// main.cpp
#include <windows.h>
#include "Account.h"
#include "EncryptionUtilities.h"
#include <fstream>
#include <iostream>

int main() {
    // �û���ԭʼ��Ϣ
    std::string userID = "1234567890";
    std::string userName = "1234";
    std::string userPassword = "password123";
    double userBalance = 1000.50;

    // �����û���Ϣ
    userID = EncryptionUtilities::caesarEncrypt(userID, 3);
    userName = EncryptionUtilities::caesarEncrypt(userName, 3);  // ��ע����ֻ������ASCII�ַ�
    userPassword = EncryptionUtilities::caesarEncrypt(userPassword, 3);

    // ��������Ϣд���ļ�
    std::ofstream outfile("accounts.dat", std::ios::binary | std::ios::app);
    if (!outfile) {
        std::cerr << "�޷����ļ�����д�롣\n";
        return -1;
    }

    outfile.write(userID.c_str(), userID.size());
    outfile.write(userName.c_str(), userName.size());
    outfile.write(userPassword.c_str(), userPassword.size());
    outfile.write(reinterpret_cast<const char*>(&userBalance), sizeof(userBalance));

    outfile.close();

    std::cout << "�û���Ϣд��ɹ���\n";


    return 0;
}