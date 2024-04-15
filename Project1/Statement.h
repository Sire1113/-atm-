// Statement.h

#pragma once

#include <string>

// ������ˮ��¼��
class Statement {
private:
    std::string ID;            // ��ˮ���
    std::string accountID;     // ��Ӧ�˻��Ŀ���
    std::string time;          // ����ʱ��
    int type;                  // �������ͣ�������ö�ٶ����������
    double amount;             // ���׽��
    std::string toAccountID;   // �Է��˻��Ŀ��ţ�ת��ʱʹ�ã�

public:
    // ���캯��
    Statement(const std::string& id, const std::string& accID,
        const std::string& tm, int ty, double amt, const std::string& toAccID)
        : ID(id), accountID(accID), time(tm), type(ty), amount(amt), toAccountID(toAccID) {}

    // ��ȡ��ˮ���
    std::string getID() const { return ID; }
    // ��ȡ�˻�����
    std::string getAccountID() const { return accountID; }
    // ��ȡ����ʱ��
    std::string getTime() const { return time; }
    // ��ȡ��������
    int getType() const { return type; }
    // ��ȡ���׽��
    double getAmount() const { return amount; }
    // ��ȡ�Է��˻�����
    std::string getToAccountID() const { return toAccountID; }

    // ������ˮ���
    void setID(const std::string& newID) { ID = newID; }
    // �����˻�����
    void setAccountID(const std::string& newAccountID) { accountID = newAccountID; }
    // ���ý���ʱ��
    void setTime(const std::string& newTime) { time = newTime; }
    // ���ý�������
    void setType(int newType) { type = newType; }
    // ���ý��׽��
    void setAmount(double newAmount) { amount = newAmount; }
    // ���öԷ��˻�����
    void setToAccountID(const std::string& newToAccountID) { toAccountID = newToAccountID; }

    // �˴��������������Ա����...
};

// ��������ö��
enum TransactionType {
    DEPOSIT = 1,   // ���
    WITHDRAWAL,    // ȡ��
    TRANSFER       // ת��
};