// seller.h
#pragma once
#include "buyer.h"
#include "item.h"
#include <vector>

class Seller : public Buyer {
private:
    std::vector<Item> stock;
public:
    Seller(int id, const std::string& name, const std::string& pass, const std::string& address, const std::string& phone, BankCustomer *account);

    void displayDetails() const override;
    
    const std::vector<Item>& getStock() const { return stock; }
    
    // Fungsi untuk mengelola stok
    void displayStock() const;
    void addItem(const Item& newItem);
    Item* findItemById(int itemId);
};