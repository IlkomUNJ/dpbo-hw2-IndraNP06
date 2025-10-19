#pragma once
#include <string>
#include <iostream>

class Item {
private:
    int id;
    std::string name;
    int quantity;
    double price;

public:
    Item(int id = 0, std::string n = "", int q = 0, double p = 0.0);
    int getId() const { return id; }
    std::string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }

    friend std::ostream& operator<<(std::ostream& os, const Item& item);
    friend std::istream& operator>>(std::istream& is, Item& item);
};