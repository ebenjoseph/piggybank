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
 	void open_account();
 	int get_balance();
 	void deposit_money(int);
 	void withdraw_money(int);
 	int get_acct_number();

};

#endif