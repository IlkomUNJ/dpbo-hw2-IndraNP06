#pragma once
#include "bank_customer.h"
#include <vector>
#include <string>

class Transaction; 
class Buyer;       

class Bank {
private:
    std::string bankName;
    std::vector<BankCustomer*> accounts;
public:
    Bank(const std::string& name = "Default Bank");
    ~Bank();

    BankCustomer* addAccount(const std::string& name, double balance);
    BankCustomer* findAccountById(int id);
    void removeAccountById(int id);
    bool transfer(BankCustomer* from, BankCustomer* to, double amount);
    void listCustomers() const; 

    void listDormantAccounts(const std::vector<Buyer*>& users, const std::vector<Transaction*>& allTransactions); 

    void saveData(const std::string& filename);
    void loadData(const std::string& filename);
};