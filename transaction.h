// transaction.h
#pragma once
#include <string>
#include <vector>
#include "item.h"
#include "utility.h"

class Transaction {
private:
    int id;
    int buyerId;
    int sellerId;
    std::vector<Item> items;
    double totalAmount;
    std::string date;
    TransactionStatus status;

public:
    Transaction(int id = 0, int bId = 0, int sId = 0, std::string d = "");
    void addItem(const Item& item);
    void calculateTotal();
    void printDetails() const;

    int getId() const { return id; }
    int getBuyerId() const { return buyerId; }
    int getSellerId() const { return sellerId; }
    const std::string& getDate() const { return date; }
    TransactionStatus getStatus() const { return status; }
    const std::vector<Item>& getItems() const { return items; }
    
    void setStatus(TransactionStatus newStatus) { status = newStatus; }

    friend std::ostream& operator<<(std::ostream& os, const Transaction& tx);
    friend std::istream& operator>>(std::istream& is, Transaction& tx);
};