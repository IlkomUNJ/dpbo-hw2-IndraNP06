// buyer.h
#pragma once
#include <string>
#include <iostream>
#include "bank_customer.h"

class Buyer {
protected: 
    int id;
    std::string name;
    std::string password; 
    std::string address;
    std::string phoneNumber;
    BankCustomer *account; 

public:
    Buyer(int id, const std::string& name, const std::string& pass, const std::string& address, const std::string& phone, BankCustomer *account);
    virtual ~Buyer() = default;

    int getId() const { return id; }
    std::string getName() const { return name; }
    bool checkPassword(const std::string& pass) const { return password == pass; }
    std::string getAddress() const { return address; }
    std::string getPhoneNumber() const { return phoneNumber; }
    BankCustomer* getAccount() { return account; }
    int getBankAccountId() const { return account->getId(); }

    virtual void displayDetails() const;
};