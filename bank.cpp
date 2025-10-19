#include "bank.h"
#include "transaction.h"
#include "utility.h"     
#include "buyer.h"      
#include <fstream>
#include <iostream>
#include <algorithm> 
#include <set>     
#include <limits>   

Bank::Bank(const std::string& name) : bankName(name) {}

Bank::~Bank() {
    for (auto* acc : accounts) {
        delete acc;
    }
    accounts.clear();
}

BankCustomer* Bank::addAccount(const std::string& name, double balance) {
    int nextId = accounts.empty() ? 1 : accounts.back()->getId() + 1;
    BankCustomer* newAcc = new BankCustomer(nextId, name, balance);
    accounts.push_back(newAcc);
    return newAcc;
}

BankCustomer* Bank::findAccountById(int id) {
    for (auto* acc : accounts) {
        if (acc->getId() == id) return acc;
    }
    return nullptr;
}

void Bank::removeAccountById(int id) {
    auto it = std::remove_if(accounts.begin(), accounts.end(), [id](BankCustomer* acc) {
        if (acc->getId() == id) {
            delete acc;
            return true;
        }
        return false;
    });

    if (it != accounts.end()) {
        accounts.erase(it, accounts.end());
    }
}

bool Bank::transfer(BankCustomer* from, BankCustomer* to, double amount) {
    if (!from || !to) {
        std::cout << "Error: Akun pengirim atau penerima tidak valid.\n";
        return false;
    }
    if (amount <= 0) {
        std::cout << "Error: Jumlah transfer harus positif.\n";
        return false;
    }
    
    if (from->withdrawBalance(amount)) {
        to->addBalance(amount);
        return true; 
    }
    
    return false; 
}


void Bank::listCustomers() const {
    std::cout << "\n--- Daftar Nasabah Bank " << bankName << " ---\n";
    if (accounts.empty()) {
        std::cout << "Belum ada nasabah yang terdaftar.\n";
        return;
    }
    for(const auto* acc : accounts) {
        acc->printInfo();
    }
}

void Bank::listDormantAccounts(const std::vector<Buyer*>& users, const std::vector<Transaction*>& allTransactions) {
    std::cout << "\n--- Daftar Akun Tidak Aktif (Tidak ada transaksi > 30 hari) ---\n";
    std::set<int> activeUserIds;
    std::string today = getCurrentDate();

    for(const auto* tx : allTransactions) {
        if (days_between(tx->getDate(), today) <= 30) {
            activeUserIds.insert(tx->getBuyerId());
            activeUserIds.insert(tx->getSellerId());
        }
    }
    
    bool foundDormant = false;
    for (const auto* user : users) {
        if (activeUserIds.find(user->getId()) == activeUserIds.end()) {
            BankCustomer* account = findAccountById(user->getBankAccountId());
            if (account) {
                account->printInfo();
                foundDormant = true;
            }
        }
    }

    if (!foundDormant) {
        std::cout << "Tidak ada akun yang tidak aktif ditemukan.\n";
    }
}


void Bank::saveData(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Gagal membuka file untuk menyimpan data bank: " << filename << std::endl;
        return;
    }
    file << accounts.size() << '\n';
    for (const auto* acc : accounts) {
        file << *acc;
    }
}

void Bank::loadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        return;
    }

    for (auto* acc : accounts) {
        delete acc;
    }
    accounts.clear();

    int count;
    file >> count;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    
    for (int i=0; i < count; ++i) {
        BankCustomer* acc = new BankCustomer();
        file >> *acc;
        if(file) { 
            accounts.push_back(acc);
        } else {
            delete acc; 
            break;      
        }
    }
}