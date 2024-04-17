// FileHandler.h

#pragma once

#include "Account.h"
#include "EncryptionUtilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Statement.h"
using namespace std;
class FileHandler {

public:
    
    // д���˻���Ϣ���ļ�
    static bool writeAccountToFile(const Account& account, int key);

    // ���ļ��ж�ȡ�˻���Ϣ����֤����
    static bool verifyAccountPassword(const  string& id, const  string& password, int key);
    // �����˻�����
    static bool updatePassword(const  string& id, const  string& oldPassword,
        const  string& newPassword, int key);
    // д�뽻�׼�¼���ļ�
    static bool writeStatementToFile(const Statement& statement, const int& key);

    // ���ļ��ж�ȡ�˻������н�����ˮ��¼
    static void readStatements(const  string& accountID, const int& key);

    // �����˻����
    static bool updateAccountBalance(const  string& id, double newBalance, int key);

    // ��ȡ�˻����
    static double getAccountBalance(const  string& id, int key);
    
    // ��ID�����˻���Ϣ
    static Account* retrieveAccountById(const  string& id, int key);
    double getRandomizedSumByAccountId(const string& encryptedAccountID, int key);
};


