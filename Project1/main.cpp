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
#include"Manage.h"
using namespace std;

int generateKey(const string &id);
int main() {
    Manage atm_manager;
  while(1){
      bool flag2 = true;
     cout << "ATMϵͳ\n";
     cout << "0. �˳�\n";
     cout << "1. ����Ա\n";
     cout << "2. �û�\n";
     cout << "��ѡ��һ��ѡ��: ";
     int option;
     cin >> option;
     switch (option) {
         
     case 1:
         
         cout << "1.�û���Ϣ" << endl;
         cout << "2.�����û�" << endl;
         cout << "0.����" << endl;
         while (flag2) {
         int option3;
         cin >> option3;
         switch (option3) {
         case 1:break;
         case 2: {
             atm_manager.createAccount();
             break;
         };
         case 0:flag2 = false; break;
         }
     }break;
         
     case 2:
     {
         int loginAttempts = 0;//��¼���������
         string id;
         int key;
         Account* currentAccount;
         
         do {
             cout << "��������ȷ�Ŀ���: ";
             cin >> id;
             key = generateKey(id);            
             currentAccount = FileHandler::retrieveAccountById(id, key);

         } while (currentAccount == nullptr);
         
         while (!(atm_manager.verifyLogin(id))) {//�ȵ�¼
             loginAttempts++;

             if (loginAttempts < 3) {
                 cout << "��������������Ѿ������ " << loginAttempts << " �Σ���ʣ" << 3 - loginAttempts << "�λ��ᣡ" << endl;
             }
             else {
                 cout << "������" << endl;
                 // ���������������˻��򿨵��߼�
                 return 1; // �˳��������Ҫ����
             }
         }
         //�������Ѿ��ɹ���¼    
         cout << "ATMϵͳ\n";
         cout << "0. ����\n";
         cout << "1. ���\n";
         cout << "2. ȡ��\n";
         cout << "3. ת��\n";
         cout << "4. ����ѯ\n";
         cout << "5. ��ʷ���ײ�ѯ\n";
         cout << "6. �޸�����\n";
         cout << "7. Ԥ�����\n";
         cout << "��ѡ��һ��ѡ��: ";
         bool flag = true;
         while (flag) {
             int option2;
             cin >> option2;
             switch (option2) {
             case 1: {   // ���
                 double depositAmount;
                 cout << "����������:";
                 cin >> depositAmount;
                 atm_manager.deposit(*currentAccount, depositAmount, key);  // ʹ�ô���
                 break;
             }
             case 2: {   // ȡ��
                 double withdrawAmount;
                 cout << "������ȡ����:";
                 cin >> withdrawAmount;
                 atm_manager.withdraw(*currentAccount, withdrawAmount, key);  // ʹ��ȡ���
                 break;
             }
             case 3: {
                 string  toAccountId;
                 double amount;
                 cout << "�������տ�˺�: ";
                 cin >> toAccountId;
                 cout << "������ת�˽��: ";
                 cin >> amount;

                 // ִ��ת��
                 bool success = atm_manager.transfer(id, toAccountId, amount);
                 if (success) {
                     cout << "ת�˳ɹ���\n";
                 }
                 else {
                     cout << "ת��ʧ�ܡ�\n";
                 }
             }
             case 4: {//����ѯ
                 atm_manager.queryBalance(id, key);
                 break;
             }
             case 5: {//��ѯ������ʷ��¼
                 atm_manager.queryTransactionHistory(id, key);
                 break;
             }
             case 6: {//�޸�����
                 atm_manager.changePassword(id, key);
                 break;
             }
             case 7: {
             atm_manager.predictBal(id,key, currentAccount->getBalance());
             break;
             }
             case 0: {//����
                 flag = false;
                 cout<< "���سɹ�,����������ѡ��" << endl;
                 
                 break;
             }
             default: {
                 cout << "������������������" << endl;
                 break;
             }
             }
         }break;
     }
     case 0: {
           cout << "ϵͳ���˳�����лʹ��" << endl;
           exit(0);
           break;}
     default: {
         cout << "������������������" << endl;
         break;
     }
    }
}
    return 0;
}




