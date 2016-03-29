
#include "bank-account.h"

using namespace std;

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