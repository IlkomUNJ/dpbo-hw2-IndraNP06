#pragma once
#include <string>
#include <iostream>

class BankCustomer {
private:
    int id;
    std::string name;
    double balance;

public:
    BankCustomer(int id=0, const std::string& name="", double balance=0.0);
    
    int getId() const;
    std::string getName() const;
    double getBalance() const;
    void printInfo() const;
    void addBalance(double amount);
    bool withdrawBalance(double amount);

    friend std::ostream& operator<<(std::ostream& os, const BankCustomer& acc);
    friend std::istream& operator>>(std::istream& is, BankCustomer& acc);
};