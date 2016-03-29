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
#include "bank-account.h"

using namespace std;

bool negative_check(int num) {
	if (num < 0) {
		cout << "Can't use negative numbers!" << endl;
		return true;
	}
	return false;
}

void create_account() {
	BankAccount ba;
	ofstream accountFile;
	ba.open_account();
	if (negative_check(ba.get_balance())) {
		cout << "Account not created - please enter an initial deposit >0" << endl;
		return;
	}
	ostringstream oss;
	oss << "account" << ba.get_acct_number() << ".piggy";
	accountFile.open(oss.str());
	accountFile.write(reinterpret_cast<char *> (&ba), sizeof(BankAccount));
	accountFile.close();
}

void display_balance(int acct) {
	BankAccount ba;
	ifstream accountFile;
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
		if (negative_check(num)) return;
		ba.deposit_money(num);
	} else {
		cout << "Enter the amount to withdraw: " << endl;
		cin >> num;
		if (negative_check(num)) return;
		if (num > ba.get_balance()) {
			cout << "Attempting to withdraw more money than you have... try again" << endl;
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
	if (negative_check(num)) return;
	if (num > ba1.get_balance()) {
		cout << "Attempting to transfer more money than you have... try again" << endl;
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

int main(int argc, char *argv[]) {
	char ch;
	int acct;
	int transferAcct;
	do
	{
		cout<<"\n\n\n\tMAIN MENU";
		cout<<"\n\n\t01. OPEN ACCOUNT";
		cout<<"\n\n\t02. DEPOSIT";
		cout<<"\n\n\t03. WITHDRAW";
		cout<<"\n\n\t04. BALANCE INQUIRY";
		cout<<"\n\n\t05. TRANSFER MONEY";
		cout<<"\n\n\t06. EXIT";
		cout<<"\n\n\tSelect Your Option (1-6) ";
		cin>>ch;
		switch(ch) {
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
		 default: cout << "Please enter a valid selection\n-- Press enter to continue --" << endl;
		}
		cin.ignore();
		cin.get();
	} while(ch!='6');

	return 0;
}