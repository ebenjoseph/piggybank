#ifndef _bank_account_

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

class BankAccount {
	size_t acctNumber;
	char name[100];
	int deposit;
	
 public:
 	void openAccount();
 	int getBalance();
 	void depositMoney(int);
 	void withdrawMoney(int);
 	int getAcctNumber();

};

#endif