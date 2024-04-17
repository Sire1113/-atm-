#include "FileHandler.h"
//���˻�д���ļ�
bool FileHandler::writeAccountToFile(const Account& account, int key) {
    ofstream fileOut("accounts.dat", ios::app); // ʹ��׷��ģʽ
    if (!fileOut) {
        cerr << "�޷����ļ�д�롣\n";
        return false;
    }
    fileOut << EncryptionUtilities::encryptDecrypt(account.getID(), key) << " "
        << account.getName() << " "
        << EncryptionUtilities::encryptDecrypt(account.getPassword(), key) << " "
        << account.getBalance() << endl;
    fileOut.close();
    return true;
}

// ���ļ��ж�ȡ�˻���Ϣ����֤����
bool FileHandler::verifyAccountPassword(const  string& id, const  string& password, int key) {
    ifstream fileIn("accounts.dat");
    if (!fileIn) {
        cerr << "�޷����ļ���ȡ��\n";
        return false;
    }

    string line;
    while (getline(fileIn, line)) {
        istringstream iss(line);
        string fileID, filePassword, fileName;
        double fileBalance;
        if (iss >> fileID >> fileName >> filePassword >> fileBalance) {
            if (fileID == EncryptionUtilities::encryptDecrypt(id, key) &&
                EncryptionUtilities::encryptDecrypt(filePassword, -key) == password) {
                fileIn.close();
                return true;
            }
        }
    }
    fileIn.close();
    return false;
}
// �����˻�����
bool FileHandler::updatePassword(const  string& id, const  string& oldPassword, const  string& newPassword, int key) {
    // ʹ��һ����ʱ�ļ��洢�µ��˻���Ϣ
    string tempFilename = "accounts_temp.dat";
    ifstream file("accounts.dat");
    ofstream tempFile(tempFilename);

    bool isUpdated = false;
    if (file.is_open() && tempFile.is_open()) {
        string fileID, fileName, filePassword;
        double fileBalance;

        while (file >> fileID >> fileName >> filePassword >> fileBalance) {
            if (fileID == EncryptionUtilities::encryptDecrypt(id, key)) {
                // ���ܲ��Ƚ�����
                string decryptedPassword = EncryptionUtilities::encryptDecrypt(filePassword, -key);
                if (decryptedPassword == oldPassword) {
                    // ����������ȷ��ʹ�������벢���ܺ�д����ʱ�ļ�
                    isUpdated = true;
                    filePassword = EncryptionUtilities::encryptDecrypt(newPassword, key);
                }
                else {
                    // �����벻��ȷ����������Ϣ������ʧ��
                    cerr << "�����벻��ȷ���޷����¡�\n";
                    return false;
                }
            }
            // ���˻���Ϣд����ʱ�ļ���
            tempFile << fileID << " " << fileName << " " << filePassword << " " << fileBalance << "\n";
        }

        file.close();
        tempFile.close();

        if (isUpdated) {
            // ʹ���µ��˻��ļ��滻�ɵ��ļ�
            remove("accounts.dat");
            rename(tempFilename.c_str(), "accounts.dat");
        }
        else {
            // ��û�и����κ����ݣ�ɾ����ʱ�ļ�
            remove(tempFilename.c_str());
            cerr << "û���ҵ���Ӧ���˻����и��¡�\n";
        }

    }
    else {
        cerr << "�޷����ļ����и��¡�\n";
        return false;
    }
    return isUpdated;
}
// д�뽻�׼�¼���ļ�
bool FileHandler::writeStatementToFile(const Statement& statement, const int& key) {
    ofstream fileOut("statements.dat", ios::app); // ʹ��׷��ģʽ�򿪽�����ˮ��¼�ļ�
    if (!fileOut) {
        cerr << "�޷����ļ�д�뽻�׼�¼��\n";
        return false;
    }

    // ���뽻����Ϣ����ʽ��Ϊ��ˮ��� �˻����� ����ʱ�� �������� ���׽�� �Է��˻�
    fileOut << statement.getID() << "\t"
        << EncryptionUtilities::encryptDecrypt(statement.getAccountID(), generateKey(statement.getAccountID())) << " "
        << statement.getTime() << " "
        << statement.getType() << " "
        << statement.getAmount() << " "
        << EncryptionUtilities::encryptDecrypt(statement.getToAccountID(),generateKey(statement.getToAccountID())) << endl;
    fileOut.close();
    return true;
}

// ���ļ��ж�ȡ�˻������н�����ˮ��¼
void FileHandler::readStatements(const  string& accountID,const int &key) {
    ifstream fileIn("statements.dat");
    if (!fileIn) {
        cerr << "�޷����ļ���ȡ���׼�¼��\n";
        return;
    }

    string line;
    cout << "��ˮ��� �˻����� ����ʱ�� �������� ���׽�� �Է��˻�\n";
    while (getline(fileIn, line)) {

        istringstream iss(line);
        string id, accID, time, toAccID;
        int type;
        double amount;
        if (iss >> id >> accID >> time >> type >> amount >> toAccID) {
            // cout << 1 <<  endl;
            if (EncryptionUtilities::encryptDecrypt(accID,-key) == accountID || EncryptionUtilities::encryptDecrypt(toAccID,-key)==accountID) {
                cout << id << " " << EncryptionUtilities::encryptDecrypt(accID, -key) << " " << time << " " << type << " " << amount << " " << EncryptionUtilities::encryptDecrypt(toAccID, -key) << endl;
            }
            else {
                // cout << accID <<  endl;
            }
        }
    }
    fileIn.close();
}

// �����˻����
bool FileHandler::updateAccountBalance(const  string& id, double newBalance, int key) {
    ifstream fileIn("accounts.dat");
    ofstream fileOut("accounts_temp.dat");
    bool isUpdated = false;

    if (fileIn && fileOut) {
        string fileID, fileName, filePassword;
        double fileBalance;
        while (fileIn >> fileID >> fileName >> filePassword >> fileBalance) {
            if (fileID == EncryptionUtilities::encryptDecrypt(id, key)) {
                fileBalance = newBalance;  // �������
                isUpdated = true;
            }
            fileOut << fileID
                << " " << fileName << " "
                << filePassword << " "
                << fileBalance << endl;
        }
        fileIn.close();
        fileOut.close();

        // ɾ�����˻��ļ�������ʱ�ļ�������Ϊ���˻��ļ�
        remove("accounts.dat");
        rename("accounts_temp.dat", "accounts.dat");
    }
    else {
        cerr << "�޷����ļ����и��¡�\n";
        if (fileIn) fileIn.close();
        if (fileOut) {
            fileOut.close();
            remove("accounts_temp.dat");  // ɾ����ʱ�ļ�
        }
        return false;
    }
    return isUpdated;
}

// ��ȡ�˻����
double FileHandler::getAccountBalance(const  string& id, int key) {
    ifstream fileIn("accounts.dat");
    if (!fileIn) {
        cerr << "�޷����ļ�����ȡ��\n";
        return -1; // ʹ�ô���ֵ��ʾ�޷���ȡ���
    }

    string fileID, fileName, filePassword;
    double fileBalance;
    while (fileIn >> fileID >> fileName >> filePassword >> fileBalance) {
        if (fileID == EncryptionUtilities::encryptDecrypt(id, key)) {
            fileIn.close();
            return fileBalance; // ���ؽ��ܺ���˻����
        }
    }
    fileIn.close();
    cerr << "û�ҵ���Ӧ���˻���\n";
    return -1; // ʹ�ô���ֵ��ʾ�˻�������
}

// ��ID�����˻���Ϣ
Account* FileHandler::retrieveAccountById(const  string& id, int key) {
    ifstream fileIn("accounts.dat");
    if (!fileIn) {
        cerr << "�޷����ļ���ȡ�˻���Ϣ��\n";
        return nullptr; // ���� nullptr ��ʾû�ҵ��˻�
    }

    string fileID, fileName, filePassword;
    double fileBalance;
    while (fileIn >> fileID >> fileName >> filePassword >> fileBalance) {
        // ���ܲ����ID�Ƿ�ƥ��
        if (EncryptionUtilities::encryptDecrypt(fileID, -key) == id) {
            fileIn.close();
            // ���ض�̬����Ķ�Ӧ���˻�����
            return new Account(id, fileName, filePassword, fileBalance);
        }
    }

    fileIn.close();
    cerr << "δ�ҵ�ƥ����˻�ID��\n";
    return nullptr; // δ�ҵ��˻������� nullptr
}


