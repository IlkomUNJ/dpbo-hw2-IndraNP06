#include "seller.h"
#include "utility.h" 
#include <iostream>
#include <iomanip>

Seller::Seller(int id, const std::string& name, const std::string& pass, const std::string& address, const std::string& phone, BankCustomer* account)
    : Buyer(id, name, pass, address, phone, account) {}

void Seller::displayDetails() const {
    Buyer::displayDetails();
    std::cout << "\b\b\b\b\b\bSeller\n";
    std::cout << "Items in stock: " << stock.size() << std::endl;
}

void Seller::displayStock() const {
    std::cout << "\n--- Stok Barang Milik " << name << " ---\n";
    if (stock.empty()) {
        std::cout << "Stok kosong.\n";
        return;
    }
    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Nama Barang" << std::setw(10) << "Harga" << std::setw(10) << "Jumlah" << std::endl;
    std::cout << "--------------------------------------------------\n";
    for (const auto& item : stock) {
        std::cout << std::left << std::setw(5) << item.getId() 
                  << std::setw(20) << item.getName() 
                  << std::setw(10) << item.getPrice() 
                  << std::setw(10) << item.getQuantity() << std::endl;
    }
}

void Seller::addItem(const Item& newItem) {
    stock.push_back(newItem);
}

Item* Seller::findItemById(int itemId) {
    for (auto& item : stock) {
        if (item.getId() == itemId) {
            return &item;
        }
    }
    return nullptr;
}