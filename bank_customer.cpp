#include "bank_customer.h"
#include <iostream>

BankCustomer::BankCustomer(int id, const std::string& name, double balance) 
    : id(id), name(name), balance(balance) {}

int BankCustomer::getId() const { return id; }
std::string BankCustomer::getName() const { return name; }
double BankCustomer::getBalance() const { return balance; }
void BankCustomer::addBalance(double amount) { balance += amount; }
bool BankCustomer::withdrawBalance(double amount) {
    if (amount > balance) return false;
    balance -= amount;
    return true;
}
void BankCustomer::printInfo() const {
    std::cout << "  ID Akun: " << id << ", Nama: " << name << ", Saldo: " << balance << std::endl;
}

std::ostream& operator<<(std::ostream& os, const BankCustomer& acc) {
    os << acc.id << '\n' << acc.name << '\n' << acc.balance << '\n';
    return os;
}
std::istream& operator>>(std::istream& is, BankCustomer& acc) {
    std::string line;
    if (std::getline(is, line)) {
        acc.id = std::stoi(line);
        std::getline(is, acc.name);
        std::getline(is, line);
        acc.balance = std::stod(line);
    }
    return is;
}