
#include "bank-account.h"

using namespace std;

void BankAccount::openAccount() {
	cout << "Enter your name:" << endl;
	cin.ignore();
	cin.getline(name, 100);
	cout << "Enter initial deposit: " << endl;
	cin >> deposit;
	acctNumber = hash<string>()(name) % 1000;
	cout << "Your account ID is: " << acctNumber << " Remember it and keep it safe!" << endl;
}

int BankAccount::getBalance() {
	return deposit;
}

void BankAccount::depositMoney(int num) {
	deposit += num;
}

void BankAccount::withdrawMoney(int num) {
	deposit -= num;
}

int BankAccount::getAcctNumber() {
	return acctNumber;
}