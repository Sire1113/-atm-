// main.cpp
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

// ��ȡ��ǰʱ�亯��ʵ��
std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now(); // ��ȡ��ǰʱ���
    auto nowTimeT = std::chrono::system_clock::to_time_t(now); // ת��Ϊtime_t
    std::tm localTime;

    // ʹ��localtime_s����localtime
    localtime_s(&localTime, &nowTimeT);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y%m%d%H%M%S"); // ��ʽ�����
    return oss.str();
}

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
bool verifyLogin(std::string id) {
    std::string  password;
    std::cout << "����������: ";
    std::cin >> password;

    if (FileHandler::verifyAccountPassword(id, password, 5)) {
        std::cout << "������֤�ɹ�����ӭ��½��\n";
        return true;
    }
    else {
        std::cout << "������֤ʧ�ܣ����Ż��������\n";
        return false;
    }
}

// ʹ�ö����Ĵ�ȡ���
void deposit(Account& account, double amount, int key) {
    double currentBalance = FileHandler::getAccountBalance(account.getID(), key);
    // �������Ƿ���Ч�Ҵ���Ϊ��
    if (currentBalance >= 0 && amount > 0) {
        account.deposit(amount);
        FileHandler::updateAccountBalance(account.getID(), account.getBalance(), key);
        // �����˻������ٴδ�����д�� Statement
        Statement depositStatement(getCurrentTime(), account.getID(),
            getCurrentTime(), TransactionType::DEPOSIT, amount, "self");
        FileHandler::writeStatementToFile(depositStatement);
    }
    else {
        std::cout << "���ʧ�ܡ�" << std::endl;
    }
}

void withdraw(Account& account, double amount, int key) {
    double currentBalance = FileHandler::getAccountBalance(account.getID(), key);
    // ���ȡ�����Ƿ�Ϸ�
    if (currentBalance >= amount && amount > 0) {
        account.withdraw(amount);
        FileHandler::updateAccountBalance(account.getID(), account.getBalance(), key);
        // �����˻������ٴδ�����д�� Statement
        Statement withdrawStatement(getCurrentTime(), account.getID(),
            getCurrentTime(), TransactionType::WITHDRAWAL, amount, "self");
        FileHandler::writeStatementToFile(withdrawStatement);
    }
    else {
        std::cout << (amount <= 0 ? "��������Ч�Ľ�\n" : "���㣬ȡ��ʧ�ܡ�\n");
    }
}
// ��ѯ���
void queryBalance(const std::string& accountId, int key) {
    // ���� FileHandler ������ȡ�˻����
    double balance = FileHandler::getAccountBalance(accountId, key);
    if (balance >= 0) {
        std::cout << "��ǰ����ǣ�" << balance << std::endl;
    }
    else {
        std::cout << "��ѯ���ʧ�ܡ�" << std::endl;
    }
}

//��ѯ������ʷ��¼
void queryTransactionHistory(const std::string& accountId) {
    std::cout << "��ѯ������ʷ��¼��...\n";
    // ���� FileHandler �ķ�������ȡ����ӡ���׼�¼
    FileHandler::readStatements(accountId);
}

/*
void transfer(Account& fromAccount, Account& toAccount, double amount, int key) {
    // ... (֮ǰ�Ŀۿ�ʹ���߼�)

    // ת�˳ɹ����¼������ˮ
    if (fromAccountBalanceUpdated && toAccountBalanceUpdated) {
        // ��ȡϵͳ��ǰʱ����Ϊ������ˮ���
        std::string transactionId = getCurrentTime(); // ���� getCurrentTime �Ѿ�ʵ��

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

        std::cout << "ת�˽����Ѽ�¼��\n";
    }
}
*/

bool transfer(const std::string& fromAccountId, const std::string& toAccountId, double amount, int key) {
    // ��������˻�
    Account* fromAccount = FileHandler::retrieveAccountById(fromAccountId, key);
    if (fromAccount == nullptr) {
        std::cerr << "����˻������ڡ�\n";
        return false;
    }

    // �����տ�˻�
    Account* toAccount = FileHandler::retrieveAccountById(toAccountId, key);
    if (toAccount == nullptr) {
        std::cerr << "�տ�˻������ڡ�\n";
        delete fromAccount; // �ͷ��ѷ���ĸ����Դ
        return false;
    }

    // ִ��ת��
    if (fromAccount->withdraw(amount)) {
        if (toAccount->deposit(amount)) {
            // ���������˻������
            FileHandler::updateAccountBalance(fromAccount->getID(), fromAccount->getBalance(), key);
            FileHandler::updateAccountBalance(toAccount->getID(), toAccount->getBalance(), key);

            // �������׼�¼��д���ļ�
            Statement fromStatement(getCurrentTime(), fromAccountId, getCurrentTime(), TransactionType::TRANSFER, -amount, toAccountId);
            FileHandler::writeStatementToFile(fromStatement);
            /*
            Statement toStatement(getCurrentTime(), toAccountId, getCurrentTime(), TransactionType::TRANSFER, amount, fromAccountId);
            FileHandler::writeStatementToFile(toStatement);
            */
            delete fromAccount; // �ͷ���Դ
            delete toAccount;
            std::cerr << "ת�˳ɹ���\n";
            return true;
        }
        else {
            // ���տ���ʧ�ܣ��ع������ȡ��
            fromAccount->deposit(amount);
        }
    }

    // ������Դ
    delete fromAccount;
    delete toAccount;
    return false;
}


int main()
{
    
    std::cout << "ATMϵͳ\n";
    std::cout << "0. �˳�\n";
    std::cout << "1. ����Ա\n";
    std::cout << "2. �û�\n";
    std::cout << "��ѡ��һ��ѡ��: ";
    int option;
    std::cin >> option;
    switch (option) {
    case 1:
        createAccount();
        break;
    case 2:
    {
        int loginAttempts = 0;//��¼���������
        std::string id;
        int key = 5;
        Account* currentAccount;
        do {
            std::cout << "��������ȷ�Ŀ���: ";
            std::cin >> id;

            currentAccount = FileHandler::retrieveAccountById(id, key);
        } while (currentAccount == nullptr);
       

            // �ǵò�����ɺ�ɾ�� Account �������ͷ��ڴ�
            //delete currentAccount;
        
        while (!verifyLogin(id)) {//�ȵ�¼
            loginAttempts++;

            if (loginAttempts < 3) {
                std::cout << "��������������Ѿ������ " << loginAttempts << " �Σ���ʣ" << 3 - loginAttempts << "�λ��ᣡ" << std::endl;
            }
            else {
                std::cout << "������" << std::endl;
                // ���������������˻��򿨵��߼�
                return 1; // �˳��������Ҫ����
            }
        }
        //�������Ѿ��ɹ���¼    
        std::cout << "ATMϵͳ\n";
        std::cout << "0. ����\n";
        std::cout << "1. ���\n";
        std::cout << "2. ȡ��\n";
        std::cout << "3. ת��\n";
        std::cout << "4. ����ѯ\n";
        std::cout << "5. ��ʷ���ײ�ѯ\n";
        std::cout << "6. �޸�����\n";
        std::cout << "��ѡ��һ��ѡ��: ";
        int option2;
        std::cin >> option2;
        switch (option2) {
        case 1: {   // ���
            double depositAmount;
            std::cout << "����������:";
            std::cin >> depositAmount;
            deposit(*currentAccount, depositAmount,key);  // ʹ�ô���
            break;
        }
        case 2: {   // ȡ��
            double withdrawAmount;
            std::cout << "������ȡ����:";
            std::cin >> withdrawAmount;
            withdraw(*currentAccount, withdrawAmount ,key);  // ʹ��ȡ���
            break;
        }
        case 3: {
            std::string  toAccountId;
            double amount;
            std::cout << "�������տ�˺�: ";
            std::cin >> toAccountId;
            std::cout << "������ת�˽��: ";
            std::cin >> amount;

            // ִ��ת��
            bool success = transfer(id, toAccountId, amount, key);
            if (success) {
                std::cout << "ת�˳ɹ���\n";
            }
            else {
                std::cout << "ת��ʧ�ܡ�\n";
            }
        }
        case 4: {//����ѯ
            queryBalance(id, key);
            break;
        }
        case 5: {//��ѯ������ʷ��¼
            queryTransactionHistory(id);
            break;
        }
        case 6: {//�޸�����
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
            break;
        }
        case 0:
            break;
        }
    }
    case 0:
        break;
    }
    return 0;
}


/*
int main() {
    
    std::string id1="0001", id2 = "123", time="123456", toAccountID="123456789",name="123",password="123",id3="1234";
    double amount = 1, balance = 1000;
    int type = 1,key=5;
    Statement newStatement(id1,id2,time,type,amount, toAccountID);
    Account newAccount(id2, name, password, balance);
    withdraw(newAccount,amount,key);
    std::cout <<FileHandler::getAccountBalance(id2,key);
    FileHandler::readStatements(id2);
    transfer(id2, id3, 400, key);
}*/

