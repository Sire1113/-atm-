// Account.h

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
// �û��˻���Ϣ��
class Account {
public:
    string ID;          // ����
    string name;        // �û�����
    string password;    // �û����루�Ѽ��ܣ�
    double balance;          // �˻����

public:
    // ���캯��
    Account(const  string& id, const  string& Name, const  string& pw, double bal);
    ~Account();//��������

    // ��ȡ����
    string getID() const;
    // ��ȡ�û�����
    string getName() const;
    // ��ȡ����
    string getPassword() const;
    // ��ȡ�˻����
    double getBalance() const;
    //����key
   // static int generateKey(const  string& ID);
    // ���ÿ���
    void setID(const  string& newID);
    // �����û�����
    void setName(const  string& newName);
    // ��������
    void setPassword(const  string& newPw);
    // �����˻����
    void setBalance(double newBal);

    // ����
    bool deposit(double amount);

    // ȡ���
    bool withdraw(double amount);
private:
    void updateBalance(double newBalance);

};
int generateKey(const  string& id);