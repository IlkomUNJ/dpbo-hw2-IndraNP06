#include "transaction.h"
#include <limits>

Transaction::Transaction(int id, int bId, int sId, std::string d)
    : id(id), buyerId(bId), sellerId(sId), totalAmount(0.0), date(d), status(TransactionStatus::PAID) {}

void Transaction::addItem(const Item& item) {
    items.push_back(item);
    calculateTotal();
}
void Transaction::calculateTotal() {
    totalAmount = 0;
    for (const auto& item : items) {
        totalAmount += item.getPrice() * item.getQuantity();
    }
}
void Transaction::printDetails() const {
    std::cout << "  Tx ID: " << id << ", Tanggal: " << date << ", Status: " << statusToString(status) << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Transaction& tx) {
    os << tx.id << '\n' << tx.buyerId << '\n' << tx.sellerId << '\n'
       << tx.totalAmount << '\n' << tx.date << '\n' << statusToString(tx.status) << '\n';
    os << tx.items.size() << '\n';
    for (const auto& item : tx.items) {
        os << item;
    }
    return os;
}
std::istream& operator>>(std::istream& is, Transaction& tx) {
    std::string line;
    if (std::getline(is, line) && !line.empty()) {
        tx.id = std::stoi(line);
        is >> tx.buyerId >> tx.sellerId >> tx.totalAmount;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(is, tx.date);
        std::getline(is, line);
        tx.status = stringToStatus(line);
        
        int itemCount;
        is >> itemCount;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        tx.items.clear();
        for (int i = 0; i < itemCount; ++i) {
            Item tempItem;
            is >> tempItem;
            tx.items.push_back(tempItem);
        }
    }
    return is;
}