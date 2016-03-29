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

bool negativeCheck(int num) {
	if (num < 0) {
		cout << "Can't use negative numbers!" << endl;
		return true;
	}
	return false;
}

bool runSubMenu() {
	char ch;
	cout << "Would you like to run another transation? (Y/N)" << endl;
	do {
		cin >> ch;
		ch = toupper(ch);
		switch(ch) {
		case 'Y':
			return false;
		case 'N':
			cout << "Thank you for using PiggyBank" << endl;
			return true;
		default: cout << "\nPlease enter a valid selection\n" << endl;
		}
		cin.ignore();
		cin.get();
	} while(ch!='Y');
	
	return false;
}

void createAccount() {
	BankAccount ba;
	ofstream accountFile;
	ba.openAccount();
	if (negativeCheck(ba.getBalance())) {
		cout << "Account not created - please enter an initial deposit >0" << endl;
		return;
	}
	ostringstream oss;
	oss << "account" << ba.getAcctNumber() << ".piggy";
	accountFile.open(oss.str().c_str());
	accountFile.write(reinterpret_cast<char *> (&ba), sizeof(BankAccount));
	accountFile.close();
}

void displayBalance(BankAccount ba) {
	cout << "\nYour current balance is: " << ba.getBalance() << endl;
}

void depositWithdraw(BankAccount ba, fstream *accountFile, bool depositing) {
	int num = 0;

	cout << "\nYour current balance is: " << ba.getBalance() << endl;
	if (depositing) {
		cout << "Enter the amount to deposit: " << endl;
		cin >> num;
		if (negativeCheck(num)) return;
		ba.depositMoney(num);
	} else {
		cout << "Enter the amount to withdraw: " << endl;
		cin >> num;
		if (negativeCheck(num)) return;
		if (num > ba.getBalance()) {
			cout << "Attempting to withdraw more money than you have... try again" << endl;
			return;
		}
		ba.withdrawMoney(num);
	}

	int pos = -1 * static_cast<int>(sizeof(BankAccount));
	accountFile->seekp(pos, ios::cur);
	accountFile->write(reinterpret_cast<char *> (&ba), sizeof(BankAccount));

	cout << "\nYour new balance is: " << ba.getBalance() << endl;
}

void transfer(BankAccount senderAccount, fstream *accountFile, int transferAcct) {
	int num = 0;
	BankAccount recipientAccount;

	fstream recipientAccountFile;
	ostringstream oss2;
	oss2 << "account" << transferAcct << ".piggy";
	recipientAccountFile.open(oss2.str().c_str());
	if (!recipientAccountFile) {
		cout << "Recipient account number is invalid" << endl;
		return;
	}
	recipientAccountFile.read(reinterpret_cast<char *> (&recipientAccount), sizeof(BankAccount));

	cout << "\nYour current balance is: " << senderAccount.getBalance() << endl;
	cout << "\nEnter the amount to transfer: " << endl;
	cin >> num;

	// error checking
	if (negativeCheck(num)) return;
	if (num > senderAccount.getBalance()) {
		cout << "Attempting to transfer more money than you have... try again" << endl;
		return;
	}

	senderAccount.withdrawMoney(num);
	recipientAccount.depositMoney(num);

	int pos = -1 * static_cast<int>(sizeof(BankAccount));
	accountFile->seekp(pos, ios::cur);
	accountFile->write(reinterpret_cast<char *> (&senderAccount), sizeof(BankAccount));

	cout << "\nYour new balance is: " << senderAccount.getBalance() << endl;

	pos = -1 * static_cast<int>(sizeof(BankAccount));
	recipientAccountFile.seekp(pos, ios::cur);
	recipientAccountFile.write(reinterpret_cast<char *> (&recipientAccount), sizeof(BankAccount));
	recipientAccountFile.close();
}

void runMainMenu(int acct) {
	char ch;
	int transferAcct;
	int pos;

	BankAccount ba;
	fstream accountFile;
	ostringstream oss;
	oss << "account" << acct << ".piggy";
	accountFile.open(oss.str().c_str());
	if (!accountFile) {
		cout << "You entered an invalid account number" << endl;
		return;
	}
	accountFile.read(reinterpret_cast<char *> (&ba), sizeof(BankAccount));

	do {
		cout << "\nMAIN MENU\n" << endl;
		cout << "1) DEPOSIT" << endl;
		cout << "2) WITHDRAW" << endl;
		cout << "3) BALANCE INQUIRY" << endl;
		cout << "4) TRANSFER MONEY" << endl;
		cout << "5) EXIT" << endl;
		cout << "Select Your Option (1-5)" << endl;
		cin >> ch;
		switch(ch) {
		case '1':
			depositWithdraw(ba, &accountFile, true);
			accountFile.close();
			if (runSubMenu()) exit(0);
			break;
		case '2':
			depositWithdraw(ba, &accountFile, false);
			accountFile.close();
			if (runSubMenu()) exit(0);
			break;
		case '3': 
			displayBalance(ba);
			accountFile.close();
			if (runSubMenu()) exit(0);
			break;
		case '4':
			cout << "Enter account to transfer to: "; cin >> transferAcct;
			transfer(ba, &accountFile, transferAcct);
			accountFile.close();
			if (runSubMenu()) exit(0);
			break;
		case '5':
			accountFile.close();
			cout << "Thank you for using PiggyBank" << endl;
			exit(0); 
		default: cout << "\nPlease enter a valid selection\n" << endl;
		}
		accountFile.open(oss.str().c_str());
		accountFile.read(reinterpret_cast<char *> (&ba), sizeof(BankAccount));
	} while(ch!='5');
}

int main(int argc, char *argv[]) {
	char ch;
	int acct;
	int transferAcct;
	do {
		cout << "\nWELCOME TO PIGGYBANK\n" << endl;
		cout << "1) OPEN ACCOUNT" << endl;
		cout << "2) LOGIN" << endl;
		cout << "3) EXIT" << endl;
		cout << "Select Your Option (1-3)" << endl;
		cin >> ch;
		switch(ch) {
		case '1':
			createAccount();
			break;
		case '2':
			cout << "Enter your account number: "; cin >> acct;
			runMainMenu(acct);
			break;
		case '3':
			cout << "Thank you for using PiggyBank" << endl;
			exit(0); 
		default: cout << "\nPlease enter a valid selection\n" << endl;
		}
	} while(ch!='3');

	return 0;
}