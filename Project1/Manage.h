#pragma once
#include <windows.h>
#include "Account.h"
#include "Statement.h"
#include <fstream>
#include <iostream>
#include "FileHandler.h"
#include <limits>
#include <string>
#include <cctype>
#include <chrono>   
#include <ctime>    
#include <sstream>  
#include <iomanip>
using namespace std;
class Manage
{
public:
	Manage();
	~Manage();
	// ��ȡ��ǰʱ�亯��ʵ��
    string getCurrentTime();

// �����˻��ĺ���
    void createAccount();


// ��֤��¼�ĺ���
    bool verifyLogin(string id);

// ʹ�ö����Ĵ�ȡ���
    void deposit(Account& account, double amount, int key);

    void withdraw(Account& account, double amount, int key);
// ��ѯ���
    void queryBalance(const  string& accountId, int key);
    void changePassword(const string& id, const int& key);
//��ѯ������ʷ��¼
void queryTransactionHistory(const  string& accountId, const int& key);

/*
void transfer(Account& fromAccount, Account& toAccount, double amount, int key) {
    // ... (֮ǰ�Ŀۿ�ʹ���߼�)

    // ת�˳ɹ����¼������ˮ
    if (fromAccountBalanceUpdated && toAccountBalanceUpdated) {
        // ��ȡϵͳ��ǰʱ����Ϊ������ˮ���
         string transactionId = getCurrentTime(); // ���� getCurrentTime �Ѿ�ʵ��

        // ����ת�����Ľ�����ˮ��¼����
        Statement outStatement(
            transactionId, // Ψһ�Ľ��ױ��
            fromAccount.getID(),
            getCurrentTime(), // ��ǰʱ��
            TransactionType::TRANSFER, // ��������
            -amount, // ת�˽���ֵ����֧����
            toAccount.getID() // �տ�˻�ID
        );

        // ��������ˮ��¼д���ļ�
        FileHandler::writeStatementToFile(outStatement);

         cout << "ת�˽����Ѽ�¼��\n";
    }
}
*/

bool transfer(const  string& fromAccountId, const  string& toAccountId, double amount);



};

