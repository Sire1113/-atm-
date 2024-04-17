#include"Manage.h"
Manage::Manage()
{
}

Manage::~Manage()
{
}
// ��ȡ��ǰʱ�亯��ʵ��
string Manage::getCurrentTime() {
    auto now = chrono::system_clock::now(); // ��ȡ��ǰʱ���
    auto nowTimeT = chrono::system_clock::to_time_t(now); // ת��Ϊtime_t
    tm localTime;

    // ʹ��localtime_s����localtime
    localtime_s(&localTime, &nowTimeT);

    ostringstream oss;
    oss << put_time(&localTime, "%Y%m%d%H%M%S"); // ��ʽ�����
    return oss.str();
}

// �����˻��ĺ���
void Manage::createAccount() {
    string id, name, password;
    double balance;
    int key;
    cout << "�����뿨��: ";
    cin >> id;
    cout << "����������: ";
    cin >> name;
    cout << "����������: ";
    cin >> password;
    cout << "�������˻���ʼ���: ";
    cin >> balance;
    key = generateKey(id);
    // �����˻�ʵ��
    Account newAccount(id, name, password, balance);

    // д���ļ�
    if (FileHandler::writeAccountToFile(newAccount, key)) {
        cout << "�˻������ɹ���\n";
    }
    else {
        cout << "�˻�����ʧ�ܣ����Ժ����ԡ�\n";
    }
}


// ��֤��¼�ĺ���
bool Manage::verifyLogin(string id) {
    string  password;
    cout << "����������: ";
    cin >> password;
    int key = generateKey(id);
    if (FileHandler::verifyAccountPassword(id, password, key)) {
        cout << "������֤�ɹ�����ӭ��½��\n";
        return true;
    }
    else {
        cout << "������֤ʧ�ܣ����Ż��������\n";
        return false;
    }
}

// ʹ�ö����Ĵ�ȡ���
void Manage::deposit(Account& account, double amount, int key) {
    
    double currentBalance = FileHandler::getAccountBalance(account.getID(), key);
    // �������Ƿ���Ч�Ҵ���Ϊ��
    if (currentBalance >= 0 && amount > 0) {
        account.deposit(amount);
        FileHandler::updateAccountBalance(account.getID(), account.getBalance(), key);
        // �����˻������ٴδ�����д�� Statement
        Statement depositStatement(getCurrentTime(), account.getID(),
            getCurrentTime(), TransactionType::DEPOSIT, amount, "self");
        FileHandler::writeStatementToFile(depositStatement,key);
    }
    else {
        cout << "���ʧ�ܡ�" << endl;
    }
}

void Manage::withdraw(Account& account, double amount, int key) {
    double currentBalance = FileHandler::getAccountBalance(account.getID(), key);
    // ���ȡ�����Ƿ�Ϸ�
    if (currentBalance >= amount && amount > 0) {
        account.withdraw(amount);
        FileHandler::updateAccountBalance(account.getID(), account.getBalance(), key);
        // �����˻������ٴδ�����д�� Statement
        Statement withdrawStatement(getCurrentTime(), account.getID(),
            getCurrentTime(), TransactionType::WITHDRAWAL, amount, "self");
        FileHandler::writeStatementToFile(withdrawStatement,key);
    }
    else {
        cout << (amount <= 0 ? "��������Ч�Ľ�\n" : "���㣬ȡ��ʧ�ܡ�\n");
    }
}
// ��ѯ���
void Manage::queryBalance(const  string& accountId, int key) {
    // ���� FileHandler ������ȡ�˻����
    double balance = FileHandler::getAccountBalance(accountId, key);
    if (balance >= 0) {
        cout << "��ǰ����ǣ�" << balance << endl;
    }
    else {
        cout << "��ѯ���ʧ�ܡ�" << endl;
    }
}

//��ѯ������ʷ��¼
void Manage::queryTransactionHistory(const  string& accountId, const int& key) {
    cout << "��ѯ������ʷ��¼��...\n";
    // ���� FileHandler �ķ�������ȡ����ӡ���׼�¼
    FileHandler::readStatements(accountId,key);
}

void Manage::changePassword(const string &id,const int & key) {
    string oldPassword, newPassword;
    cout << "oldpassword:";
    cin >> oldPassword;
    cout << "newpassword:";
    cin >> newPassword;
    bool passwordChanged = FileHandler::updatePassword(id, oldPassword, newPassword, key);
    if (passwordChanged) {
        cout << "������³ɹ���" << endl;
    }
    else {
        cout << "�������ʧ�ܡ�" << endl;
    }
}
// ת�˺���
bool Manage::transfer(const  string& fromAccountId, const  string& toAccountId, double amount) {
    // ��������˻�
    int key1 = generateKey(fromAccountId);
    int key2 = generateKey(toAccountId);
    Account* fromAccount = FileHandler::retrieveAccountById(fromAccountId, key1);
    if (fromAccount == nullptr) {
        cerr << "����˻������ڡ�\n";
        return false;
    }

    // �����տ�˻�
    Account* toAccount = FileHandler::retrieveAccountById(toAccountId, key2);
    if (toAccount == nullptr) {
        cerr << "�տ�˻������ڡ�\n";
        delete fromAccount; // �ͷ��ѷ���ĸ����Դ
        return false;
    }

    // ִ��ת��
    if (fromAccount->withdraw(amount)) {
        if (toAccount->deposit(amount)) {
            // ���������˻������
            FileHandler::updateAccountBalance(fromAccount->getID(), fromAccount->getBalance(), key1);
            FileHandler::updateAccountBalance(toAccount->getID(), toAccount->getBalance(), key2);

            // �������׼�¼��д���ļ�
            Statement fromStatement(getCurrentTime(), fromAccountId, getCurrentTime(), TransactionType::TRANSFER, -amount, toAccountId);
            FileHandler::writeStatementToFile(fromStatement,key1);
            
            delete fromAccount; // �ͷ���Դ
            delete toAccount;
            cerr << "ת�˳ɹ���\n";
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
