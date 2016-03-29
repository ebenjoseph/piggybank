#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <cassert>
#include <ctime>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <functional> 
#include <sys/wait.h>
#include <unistd.h>
#include <sstream> 
#include <sched.h>
#include "account.h"

using namespace std;

class BankAccount {
	size_t acctNumber;
	char name[100];
	int deposit;
	char type;
	
 public:
 	void open_account();
 	int get_balance();
 	void deposit_money(int);
 	void withdraw_money(int);
 	int get_acct_number();

};

void BankAccount::open_account() {
	cout << "Enter your name:" << endl;
	cin.ignore();
	cin.getline(name, 100);
	cout << "Enter initial deposit: " << endl;
	cin >> deposit;
	acctNumber = hash<string>()(name) % 1000;
	cout << "Your account ID is: " << acctNumber << " Remember it and keep it safe!" << endl;
}

int BankAccount::get_balance() {
	return deposit;
}

void BankAccount::deposit_money(int num) {
	deposit += num;
}

void BankAccount::withdraw_money(int num) {
	deposit -= num;
}

int BankAccount::get_acct_number() {
	return acctNumber;
}



///////////////////////////////////////////////////////////////

void create_account() {
	BankAccount ba;
	fstream accountFile;
	ba.open_account();
	ostringstream oss;
	oss << "account" << ba.get_acct_number() << ".piggy";
	accountFile.open(oss.str());
	accountFile.write(reinterpret_cast<char *> (&ba), sizeof(BankAccount));
	accountFile.close();
}

void display_balance(int acct) {
	BankAccount ba;
	fstream accountFile;
	ostringstream oss;
	oss << "account" << acct << ".piggy";
	accountFile.open(oss.str());
	if (!accountFile) {
		cout << "You entered an invalid account number" << endl;
		return;
	}
	accountFile.read(reinterpret_cast<char *> (&ba), sizeof(BankAccount));
	cout << "\nYour current balance is: " << ba.get_balance() << endl;
	accountFile.close();
}

void deposit_withdraw(int acct, bool depositing) {
	int num = 0;
	BankAccount ba;
	fstream accountFile;
	ostringstream oss;
	oss << "account" << acct << ".piggy";
	accountFile.open(oss.str());
	if (!accountFile) {
		cout << "You entered an invalid account number" << endl;
		return;
	}
	accountFile.read(reinterpret_cast<char *> (&ba), sizeof(BankAccount));

	cout << "\nYour current balance is: " << ba.get_balance() << endl;
	if (depositing) {
		cout << "Enter the amount to deposit: " << endl;
		cin >> num;
		if (num < 0) {
			cout << "Can't use negative numbers, dummy!" << endl;
			return;
		}
		ba.deposit_money(num);
	} else {
		cout << "Enter the amount to withdraw: " << endl;
		cin >> num;
		if (num < 0) {
			cout << "Can't use negative numbers, dummy!" << endl;
			return;
		}
		ba.withdraw_money(num);
	}

	int pos = -1 * static_cast<int>(sizeof(ba));
	accountFile.seekp(pos, ios::cur);
	accountFile.write(reinterpret_cast<char *> (&ba), sizeof(BankAccount));

	cout << "\nYour new balance is: " << ba.get_balance() << endl;
	accountFile.close();
}

void transfer(int acct, int transferAcct) {
	int num = 0;
	BankAccount ba1;
	BankAccount ba2;

	fstream accountFile;
	ostringstream oss;
	oss << "account" << acct << ".piggy";
	accountFile.open(oss.str());
	if (!accountFile) {
		cout << "Your account number is invalid" << endl;
		return;
	}
	accountFile.read(reinterpret_cast<char *> (&ba1), sizeof(BankAccount));

	fstream transferAccountFile;
	ostringstream oss2;
	oss2 << "account" << transferAcct << ".piggy";
	transferAccountFile.open(oss2.str());
	if (!transferAccountFile) {
		cout << "Transfer account number is invalid" << endl;
		return;
	}
	transferAccountFile.read(reinterpret_cast<char *> (&ba2), sizeof(BankAccount));

	cout << "\nYour current balance is: " << ba1.get_balance() << endl;
	cout << "\nEnter the amount to transfer: " << endl;
	cin >> num;
	if (num < 0) {
		cout << "Can't use negative numbers, dummy!" << endl;
		return;
	}

	ba1.withdraw_money(num);
	ba2.deposit_money(num);

	int pos = -1 * static_cast<int>(sizeof(ba1));
	accountFile.seekp(pos, ios::cur);
	accountFile.write(reinterpret_cast<char *> (&ba1), sizeof(BankAccount));

	cout << "\nYour new balance is: " << ba1.get_balance() << endl;
	accountFile.close();

	pos = -1 * static_cast<int>(sizeof(ba2));
	transferAccountFile.seekp(pos, ios::cur);
	transferAccountFile.write(reinterpret_cast<char *> (&ba2), sizeof(BankAccount));
	transferAccountFile.close();
}

int main() {
	cout << "test" << endl;
	char ch;
	int acct;
	int transferAcct;
	//intro();
	do
	{
		//system("cls");
		cout<<"\n\n\n\tMAIN MENU";
		cout<<"\n\n\t01. OPEN ACCOUNT";
		cout<<"\n\n\t02. DEPOSIT";
		cout<<"\n\n\t03. WITHDRAW";
		cout<<"\n\n\t04. BALANCE INQUIRY";
		cout<<"\n\n\t05. TRANSFER MONEY";
		cout<<"\n\n\t06. EXIT";
		cout<<"\n\n\tSelect Your Option (1-6) ";
		cin>>ch;
		//system("cls");
		switch(ch)
		{
		case '1':
			create_account();
			break;
		case '2':
			cout << "Enter your account number: "; cin >> acct;
			deposit_withdraw(acct, true);
			break;
		case '3':
			cout << "Enter your account number: "; cin >> acct;
			deposit_withdraw(acct, false);
			break;
		case '4': 
			cout << "Enter your account number: "; cin >> acct;
			display_balance(acct);
			break;
		case '5':
			cout << "Enter your account number: "; cin >> acct;
			cout << "Enter account to transfer to: "; cin >> transferAcct;
			transfer(acct, transferAcct);
			break;
		case '6':
			cout<<"Thank you for using PiggyBank" << endl;
			break;
		 default :cout<<"\a";
		}
		cin.ignore();
		cin.get();
	}while(ch!='6');

	return 0;
}