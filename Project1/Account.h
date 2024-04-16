// Account.h

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
// �û��˻���Ϣ��
class Account {
private:
    std::string ID;          // ����
    std::string name;        // �û�����
    std::string password;    // �û����루�Ѽ��ܣ�
    double balance;          // �˻����

public:
    // ���캯��
    Account(const std::string& id, const std::string& name, const std::string& pw, double bal)
        : ID(id), name(name), password(pw), balance(bal) {}

    // ��ȡ����
    std::string getID() const { return ID; }
    // ��ȡ�û�����
    std::string getName() const { return name; }
    // ��ȡ����
    std::string getPassword() const { return password; }
    // ��ȡ�˻����
    double getBalance() const { return balance; }

    // ���ÿ���
    void setID(const std::string& newID) { ID = newID; }
    // �����û�����
    void setName(const std::string& newName) { name = newName; }
    // ��������
    void setPassword(const std::string& newPw) { password = newPw; }
    // �����˻����
    void setBalance(double newBal) { balance = newBal; }

    // ����
    bool deposit(double amount) {
        if (amount > 0) {
            updateBalance(balance + amount); // ���� private ��Ա���������
            std::cout << "���ɹ�����ǰ��" << balance << std::endl;
            return true;
        }
        else {
            std::cout << "��������Ч�Ľ�\n";
            return false;
        }
    }

    // ȡ���
    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            updateBalance(balance - amount); // ���� private ��Ա���������
            std::cout << "ȡ��ɹ�����ǰ��" << balance << std::endl;
            return true;
        }
        else {
            std::cout << (amount <= 0 ? "��������Ч�Ľ�\n" : "���㣬ȡ��ʧ�ܡ�\n");
            return false;
        }
    }
private:
    void updateBalance(double newBalance) {
        balance = newBalance;
    }
};