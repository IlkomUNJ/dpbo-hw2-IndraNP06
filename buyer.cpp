#include "buyer.h"
#include <iostream>

Buyer::Buyer(int id, const std::string& name, const std::string& pass, const std::string& address, const std::string& phone, BankCustomer *account)
    : id(id), name(name), password(pass), address(address), phoneNumber(phone), account(account) {}

void Buyer::displayDetails() const {
    std::cout << "User ID: " << id << "\n"
              << "Name: " << name << "\n"
              << "Address: " << address << "\n"
              << "Phone: " << phoneNumber << "\n"
              << "Type: Buyer\n";
    if (account) {
        std::cout << "Bank Account Info:\n";
        account->printInfo();
    }
}