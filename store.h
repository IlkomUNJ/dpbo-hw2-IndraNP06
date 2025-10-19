// store.h
#pragma once
#include "buyer.h"
#include "seller.h"
#include "bank.h"
#include "transaction.h"
#include <vector>

class Store {
private:
    std::vector<Buyer*> users;
    std::vector<Transaction*> transactions;
    Bank bank;
    Buyer* currentUser = nullptr;

public:
    ~Store();
    void run();

private:
    void saveData();
    void loadData();

    void showMainMenu();
    void login();
    void registerUser();
    
    void showAdminMenu();
    void viewAllUsers();
    void viewUserDetails();
    void searchUser();
    void createUser();
    void removeUser();
    void adminReportsMenu();
    void listTransactionsLastKDays();
    void listPaidButNotCompleted();
    void listMostFrequentItems();
    void listMostActiveUsers();


    void showUserMenu();
    void logout();
    
    void showBuyerMenu();
    void purchaseItem();
    void viewMyOrders();

    void showSellerMenu();
    void manageStock();
    void viewMySales();
    void fulfillOrder();
};