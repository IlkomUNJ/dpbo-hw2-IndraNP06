#include "item.h"
#include <limits>

Item::Item(int id, std::string n, int q, double p) : id(id), name(n), quantity(q), price(p) {}

std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << item.id << '\n' << item.name << '\n' << item.quantity << '\n' << item.price << '\n';
    return os;
}

std::istream& operator>>(std::istream& is, Item& item) {
    std::string line;
    if (std::getline(is, line) && !line.empty()) {
        item.id = std::stoi(line);
        std::getline(is, item.name);
        is >> item.quantity >> item.price;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return is;
}