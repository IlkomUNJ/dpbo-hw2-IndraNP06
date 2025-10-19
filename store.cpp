#include "store.h"
#include "seller.h" 
#include "utility.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>

Store::~Store() {
    for (auto* u : users) { delete u; }
    for (auto* t : transactions) { delete t; }
}

void Store::run() {
    loadData();
    if (users.empty()) {
        std::cout << "Membuat data awal default...\n";
        BankCustomer* acc1 = bank.addAccount("indra", 1000.0);
        users.push_back(new Buyer(1, "indra", "123", "123 Maple St", "555-0101", acc1));
        
        BankCustomer* acc2 = bank.addAccount("andri", 5000.0);
        Seller* seller1 = new Seller(2, "andri", "123", "456 Oak Ave", "555-0102", acc2);
        seller1->addItem(Item(1, "Buku OOP", 10, 150.0));
        seller1->addItem(Item(2, "Pensil 2B", 50, 5.0));
        seller1->addItem(Item(3, "Penghapus", 30, 2.5));
        users.push_back(seller1);
    }
    showMainMenu();
}

void Store::saveData() {
    std::ofstream txFile("transactions.dat");
    if (txFile) {
        txFile << transactions.size() << '\n';
        for(const auto* tx : transactions) {
            txFile << *tx;
        }
    }

    bank.saveData("bank.dat");
    std::cout << "Data berhasil disimpan." << std::endl;
}

void Store::loadData() {
    std::ifstream txFile("transactions.dat");
    if (txFile) {
        int count;
        txFile >> count;
        txFile.ignore();
        for (int i=0; i < count; ++i) {
            Transaction* tx = new Transaction();
            txFile >> *tx;
            transactions.push_back(tx);
        }
    }
    bank.loadData("bank.dat");
}

void Store::showMainMenu() {
    int choice = 0;
    while (choice != 4) {
        std::cout << "\n===== SELAMAT DATANG DI TOKO ONLINE =====\n";
        std::cout << "1. Login Pengguna\n";
        std::cout << "2. Register Pengguna Baru\n";
        std::cout << "3. Admin Login\n";
        std::cout << "4. Simpan & Keluar\n";
        std::cout << ">> ";
        std::cin >> choice;

        if (std::cin.fail()) {
            clearInputBuffer();
            choice = 0;
        }

        switch(choice) {
            case 1: login(); break;
            case 2: registerUser(); break;
            case 3: 
                {
                    std::string username, password;
                    std::cout << "--- Admin Login ---\n";
                    std::cout << "Username: "; std::cin >> username;
                    std::cout << "Password: "; std::cin >> password;
                    if (username == "root" && password == "toor") {
                        std::cout << "Admin login berhasil!\n";
                        showAdminMenu();
                    } else {
                        std::cout << "Username atau password salah.\n";
                    }
                }
                break;
            case 4: 
                std::cout << "Menyimpan data sebelum keluar...\n"; 
                saveData();
                break;
            default: std::cout << "Pilihan tidak valid.\n"; break;
        }
    }
}

void Store::login() {
    std::string name, pass;
    std::cout << "\n--- Login Pengguna ---\n";
    std::cout << "Nama Pengguna: ";
    std::cin >> name;
    std::cout << "Password: ";
    std::cin >> pass;

    for (auto* user : users) {
        if (user->getName() == name && user->checkPassword(pass)) {
            currentUser = user;
            std::cout << "Login berhasil! Selamat datang, " << name << ".\n";
            showUserMenu();
            return;
        }
    }
    std::cout << "Nama pengguna atau password salah.\n";
}

void Store::registerUser() {
    std::cout << "\n--- Registrasi Pengguna Baru ---\n";
    std::cout << "Pilih tipe (1: Buyer, 2: Seller): ";
    int type;
    std::cin >> type;
    
    if (std::cin.fail() || (type != 1 && type != 2)) {
        clearInputBuffer();
        std::cout << "Tipe tidak valid. Registrasi dibatalkan.\n";
        return;
    }

    std::string name, pass, address, phone;
    double initialBalance;
    
    std::cout << "Nama Pengguna: "; std::cin >> name;
    std::cout << "Password: "; std::cin >> pass;
    std::cin.ignore(); 
    std::cout << "Alamat: "; std::getline(std::cin, address);
    std::cout << "No. Telp: "; std::getline(std::cin, phone);
    std::cout << "Saldo Awal Bank: "; std::cin >> initialBalance;

    if(std::cin.fail()){
        clearInputBuffer();
        std::cout << "Input saldo tidak valid. Registrasi dibatalkan.\n";
        return;
    }

    for(const auto* user : users) {
        if(user->getName() == name) {
            std::cout << "Nama pengguna sudah ada. Silakan gunakan nama lain.\n";
            return;
        }
    }

    BankCustomer* newAccount = bank.addAccount(name, initialBalance);
    int nextId = users.empty() ? 1 : users.back()->getId() + 1;

    if (type == 1) {
        users.push_back(new Buyer(nextId, name, pass, address, phone, newAccount));
    } else {
        users.push_back(new Seller(nextId, name, pass, address, phone, newAccount));
    }
    std::cout << "Registrasi berhasil! Silakan login.\n";
}

void Store::logout() {
    std::cout << "Anda telah berhasil logout.\n";
    currentUser = nullptr;
}

void Store::showUserMenu() {
    if (!currentUser) return;
    if (dynamic_cast<Seller*>(currentUser)) {
        showSellerMenu();
    } else {
        showBuyerMenu();
    }
}

void Store::showBuyerMenu() {
    int choice = 0;
    while(choice != 3) {
        std::cout << "\n=== Menu Pembeli: " << currentUser->getName() << " ===\n";
        std::cout << "Saldo Anda: " << currentUser->getAccount()->getBalance() << std::endl;
        std::cout << "1. Beli Barang\n";
        std::cout << "2. Lihat Riwayat Pesanan\n";
        std::cout << "3. Logout\n";
        std::cout << ">> ";
        std::cin >> choice;
        if(std::cin.fail()) { clearInputBuffer(); choice=0; }

        switch(choice){
            case 1: purchaseItem(); break;
            case 2: viewMyOrders(); break;
            case 3: logout(); break;
            default: std::cout << "Pilihan tidak valid.\n";
        }
    }
}

void Store::showSellerMenu() {
    int choice = 0;
    while(choice != 4) {
        std::cout << "\n=== Menu Penjual: " << currentUser->getName() << " ===\n";
        std::cout << "Saldo Anda: " << currentUser->getAccount()->getBalance() << std::endl;
        std::cout << "1. Kelola Stok Barang\n";
        std::cout << "2. Lihat Riwayat Penjualan\n";
        std::cout << "3. Penuhi Pesanan (Ubah status)\n";
        std::cout << "4. Logout\n";
        std::cout << ">> ";
        std::cin >> choice;
        if(std::cin.fail()) { clearInputBuffer(); choice=0; }

        switch(choice){
            case 1: manageStock(); break;
            case 2: viewMySales(); break;
            case 3: fulfillOrder(); break;
            case 4: logout(); break;
            default: std::cout << "Pilihan tidak valid.\n";
        }
    }
}


void Store::purchaseItem() {
    std::cout << "\n--- Daftar Penjual Tersedia ---\n";
    std::vector<Seller*> sellers;
    for(auto* user : users) {
        if(auto* seller = dynamic_cast<Seller*>(user)) {
            if(seller->getId() != currentUser->getId()) { 
                sellers.push_back(seller);
                std::cout << "ID: " << seller->getId() << " - Nama: " << seller->getName() << std::endl;
            }
        }
    }
    if(sellers.empty()){ std::cout << "Tidak ada penjual lain yang tersedia saat ini.\n"; return; }
    
    std::cout << "Pilih ID Penjual: ";
    int sellerId;
    std::cin >> sellerId;

    Seller* targetSeller = nullptr;
    for(auto* s : sellers) { if (s->getId() == sellerId) { targetSeller = s; break; } }
    if (!targetSeller) { std::cout << "Penjual dengan ID tersebut tidak ditemukan.\n"; return; }

    targetSeller->displayStock();
    std::vector<Item> cart;
    double totalAmount = 0;
    while(true) {
        std::cout << "Masukkan ID barang untuk dibeli (ketik 0 untuk selesai): ";
        int itemId;
        std::cin >> itemId;
        if(std::cin.fail()) { clearInputBuffer(); itemId = -1; }
        if (itemId == 0) break;
        
        Item* itemInStock = targetSeller->findItemById(itemId);
        if(!itemInStock) { std::cout << "Barang tidak ditemukan di stok penjual.\n"; continue; }

        std::cout << "Jumlah yang ingin dibeli: ";
        int quantity;
        std::cin >> quantity;
        if(std::cin.fail() || quantity <= 0 || quantity > itemInStock->getQuantity()) {
            clearInputBuffer();
            std::cout << "Jumlah tidak valid atau stok tidak mencukupi.\n";
            continue;
        }
        
        cart.push_back(Item(itemId, itemInStock->getName(), quantity, itemInStock->getPrice()));
        totalAmount += itemInStock->getPrice() * quantity;
        std::cout << "-> " << itemInStock->getName() << " x " << quantity << " ditambahkan ke keranjang.\n";
    }

    if (cart.empty()) { std::cout << "Keranjang kosong, pembelian dibatalkan.\n"; return; }

    std::cout << "\n--- Konfirmasi Pembelian ---\n";
    std::cout << "Total Belanja: " << totalAmount << "\n";
    std::cout << "Saldo Anda Saat Ini: " << currentUser->getAccount()->getBalance() << "\n";
    std::cout << "Lanjutkan pembayaran? (y/n): ";
    char confirm;
    std::cin >> confirm;
    
    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Pembayaran dibatalkan oleh pengguna.\n";
        return;
    }
    
    bool success = bank.transfer(currentUser->getAccount(), targetSeller->getAccount(), totalAmount);
    if(success) {
        int nextTxId = transactions.empty() ? 1 : transactions.back()->getId() + 1;
        Transaction* newTx = new Transaction(nextTxId, currentUser->getId(), targetSeller->getId(), getCurrentDate());
        for(const auto& itemInCart : cart) {
            newTx->addItem(itemInCart);
            Item* itemInStock = targetSeller->findItemById(itemInCart.getId());
            if(itemInStock) { itemInStock->setQuantity(itemInStock->getQuantity() - itemInCart.getQuantity()); }
        }
        transactions.push_back(newTx);
        std::cout << "Pembayaran berhasil! Transaksi dibuat dengan ID: " << newTx->getId() << std::endl;
    } else {
        std::cout << "Pembayaran gagal. Saldo Anda tidak mencukupi.\n";
    }
}

void Store::viewMyOrders() {
    std::cout << "\n--- Riwayat Pesanan Anda ---\n";
    bool found = false;
    for(const auto* tx : transactions) {
        if (tx->getBuyerId() == currentUser->getId()) {
            tx->printDetails();
            found = true;
        }
    }
    if (!found) std::cout << "Anda belum memiliki riwayat pesanan.\n";
}

void Store::manageStock() {
    auto* seller = dynamic_cast<Seller*>(currentUser);
    if (!seller) return;

    int choice = 0;
    while(choice != 3) {
        seller->displayStock();
        std::cout << "\n--- Kelola Stok ---\n";
        std::cout << "1. Tambah Barang Baru\n";
        std::cout << "2. Ubah Jumlah Stok Barang\n";
        std::cout << "3. Kembali ke Menu Utama\n";
        std::cout << ">> ";
        std::cin >> choice;
        if(std::cin.fail()){ clearInputBuffer(); choice = 0; }

        if(choice == 1) {
            std::string name;
            int qty;
            double price;
            std::cout << "Nama Barang Baru: "; std::cin.ignore(); std::getline(std::cin, name);
            std::cout << "Jumlah Stok Awal: "; std::cin >> qty;
            std::cout << "Harga per Barang: "; std::cin >> price;
            int nextItemId = seller->getStock().empty() ? 1 : seller->getStock().back().getId() + 1;
            seller->addItem(Item(nextItemId, name, qty, price));
            std::cout << "Barang baru berhasil ditambahkan!\n";
        } else if (choice == 2) {
            std::cout << "Masukkan ID Barang yang akan diubah: ";
            int id;
            std::cin >> id;
            Item* item = seller->findItemById(id);
            if(item) {
                std::cout << "Masukkan Jumlah Stok Baru: ";
                int newQty;
                std::cin >> newQty;
                item->setQuantity(newQty);
                std::cout << "Stok berhasil diperbarui.\n";
            } else {
                std::cout << "Barang tidak ditemukan.\n";
            }
        }
    }
}

void Store::viewMySales() {
    std::cout << "\n--- Riwayat Penjualan Anda ---\n";
    bool found = false;
    for(const auto* tx : transactions) {
        if (tx->getSellerId() == currentUser->getId()) {
            tx->printDetails();
            found = true;
        }
    }
    if (!found) std::cout << "Anda belum memiliki riwayat penjualan.\n";
}

void Store::fulfillOrder() {
     auto* seller = dynamic_cast<Seller*>(currentUser);
     if(!seller) return;

     std::cout << "\n--- Pesanan yang Perlu Dipenuhi (Status: PAID) ---\n";
     std::vector<Transaction*> pendingOrders;
     for(auto* tx : transactions) {
         if(tx->getSellerId() == seller->getId() && tx->getStatus() == TransactionStatus::PAID) {
             tx->printDetails();
             pendingOrders.push_back(tx);
         }
     }
     if(pendingOrders.empty()) {
         std::cout << "Tidak ada pesanan yang perlu dipenuhi saat ini.\n";
         return;
     }

     std::cout << "Masukkan ID Transaksi yang ingin diubah statusnya menjadi COMPLETED (0 untuk batal): ";
     int txId;
     std::cin >> txId;
     if(txId == 0) return;

     bool found = false;
     for(auto* tx : pendingOrders) {
         if(tx->getId() == txId) {
             tx->setStatus(TransactionStatus::COMPLETED);
             std::cout << "Status Transaksi ID " << txId << " berhasil diubah menjadi COMPLETED.\n";
             found = true;
             break;
         }
     }
     if(!found) {
         std::cout << "Transaksi dengan ID tersebut tidak ditemukan atau statusnya bukan PAID.\n";
     }
}

void Store::showAdminMenu() {
    int choice = 0;
    while (choice != 7) {
        std::cout << "\n===== ADMIN MENU =====\n";
        std::cout << "1. Lihat Semua Pengguna\n";
        std::cout << "2. Lihat Detail Pengguna\n";
        std::cout << "3. Cari Pengguna\n";
        std::cout << "4. Buat Pengguna Baru\n";
        std::cout << "5. Hapus Pengguna\n";
        std::cout << "6. Lihat Laporan Toko\n";
        std::cout << "7. Logout\n";
        std::cout << ">> ";
        std::cin >> choice;
        if (std::cin.fail()) { clearInputBuffer(); choice = 0; }
        switch (choice) {
            case 1: viewAllUsers(); break;
            case 2: viewUserDetails(); break;
            case 3: searchUser(); break;
            case 4: createUser(); break;
            case 5: removeUser(); break;
            case 6: adminReportsMenu(); break;
            case 7: std::cout << "Logout dari panel admin.\n"; break;
            default: std::cout << "Pilihan tidak valid.\n"; break;
        }
    }
}

void Store::adminReportsMenu() {
    int choice = 0;
    while(choice != 6) {
        std::cout << "\n--- Menu Laporan Admin ---\n";
        std::cout << "1. Tampilkan Transaksi 'k' Hari Terakhir\n";
        std::cout << "2. Tampilkan Transaksi PAID tapi Belum COMPLETED\n";
        std::cout << "3. Tampilkan 'm' Barang Paling Laris\n";
        std::cout << "4. Tampilkan Pengguna Paling Aktif\n";
        std::cout << "5. Tampilkan Akun Bank Tidak Aktif\n";
        std::cout << "6. Kembali ke Menu Admin\n";
        std::cout << ">> ";
        std::cin >> choice;
        if(std::cin.fail()){ clearInputBuffer(); choice = 0; }

        switch(choice){
            case 1: listTransactionsLastKDays(); break;
            case 2: listPaidButNotCompleted(); break;
            case 3: listMostFrequentItems(); break;
            case 4: listMostActiveUsers(); break;
            case 5: bank.listDormantAccounts(users, transactions); break;
            case 6: break;
            default: std::cout << "Pilihan tidak valid.\n";
        }
    }
}

void Store::listTransactionsLastKDays() {
    std::cout << "Masukkan jumlah hari (k): ";
    int k;
    std::cin >> k;
    if(std::cin.fail() || k < 0) { clearInputBuffer(); std::cout << "Input tidak valid.\n"; return; }

    std::cout << "\n--- Transaksi dalam " << k << " Hari Terakhir ---\n";
    std::string today = getCurrentDate();
    bool found = false;
    for(const auto* tx : transactions) {
        if(days_between(tx->getDate(), today) <= k) {
            tx->printDetails();
            found = true;
        }
    }
    if(!found) std::cout << "Tidak ada transaksi ditemukan.\n";
}

void Store::listPaidButNotCompleted() {
    std::cout << "\n--- Transaksi PAID Belum COMPLETED ---\n";
    bool found = false;
    for(const auto* tx : transactions) {
        if(tx->getStatus() == TransactionStatus::PAID) {
            tx->printDetails();
            found = true;
        }
    }
    if(!found) std::cout << "Tidak ada transaksi yang perlu diproses.\n";
}

void Store::listMostFrequentItems() {
    std::cout << "Masukkan jumlah barang terlaris (m): ";
    int m;
    std::cin >> m;
    if(std::cin.fail() || m <= 0) { clearInputBuffer(); std::cout << "Input tidak valid.\n"; return; }

    std::map<std::string, int> itemCounts;
    for(const auto* tx : transactions) {
        for(const auto& item : tx->getItems()) {
            itemCounts[item.getName()] += item.getQuantity();
        }
    }

    std::vector<std::pair<std::string, int>> sortedItems(itemCounts.begin(), itemCounts.end());
    std::sort(sortedItems.begin(), sortedItems.end(), [](const auto& a, const auto& b){
        return a.second > b.second;
    });

    std::cout << "\n--- Top " << m << " Barang Terlaris ---\n";
    int count = 0;
    for(const auto& pair : sortedItems) {
        if(count++ >= m) break;
        std::cout << pair.first << " - Terjual: " << pair.second << " unit\n";
    }
}

void Store::listMostActiveUsers() {
    std::map<int, int> buyerTxCounts;
    std::map<int, int> sellerTxCounts;
    for(const auto* tx : transactions) {
        buyerTxCounts[tx->getBuyerId()]++;
        sellerTxCounts[tx->getSellerId()]++;
    }

    auto findUserName = [&](int id) {
        for(const auto* u : users) { if(u->getId() == id) return u->getName(); }
        return std::string("Unknown");
    };

    std::vector<std::pair<int, int>> sortedBuyers(buyerTxCounts.begin(), buyerTxCounts.end());
    std::sort(sortedBuyers.begin(), sortedBuyers.end(), [](const auto& a, const auto& b){ return a.second > b.second; });

    std::vector<std::pair<int, int>> sortedSellers(sellerTxCounts.begin(), sellerTxCounts.end());
    std::sort(sortedSellers.begin(), sortedSellers.end(), [](const auto& a, const auto& b){ return a.second > b.second; });

    std::cout << "\n--- Pengguna Paling Aktif ---\n";
    std::cout << "Pembeli Teratas:\n";
    for(size_t i = 0; i < std::min(sortedBuyers.size(), static_cast<size_t>(5)); ++i) {
        std::cout << i+1 << ". " << findUserName(sortedBuyers[i].first) << " (" << sortedBuyers[i].second << " transaksi)\n";
    }
    std::cout << "Penjual Teratas:\n";
    for(size_t i = 0; i < std::min(sortedSellers.size(), static_cast<size_t>(5)); ++i) {
        std::cout << i+1 << ". " << findUserName(sortedSellers[i].first) << " (" << sortedSellers[i].second << " transaksi)\n";
    }
}

void Store::viewAllUsers() {
    std::cout << "\n--- Daftar Semua Pengguna ---\n";
    if (users.empty()) { std::cout << "Tidak ada pengguna di sistem.\n"; return; }
    for (const auto* user : users) {
        std::string userType = (dynamic_cast<const Seller*>(user)) ? "Seller" : "Buyer";
        std::cout << "ID: " << user->getId() << ", Nama: " << user->getName() << ", Tipe: " << userType << std::endl;
    }
}

void Store::viewUserDetails() {
    std::cout << "\n--- Informasi Detail Semua Pengguna ---\n";
    if (users.empty()) { std::cout << "Tidak ada pengguna di sistem.\n"; return; }
    for (const auto* user : users) {
        user->displayDetails();
        std::cout << std::endl;
    }
}

void Store::searchUser() {
    std::cout << "\n--- Cari Pengguna ---\n";
    std::cout << "Masukkan kata kunci (ID, Nama, Alamat, atau No. Telp): ";
    std::string searchTerm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, searchTerm);
    
    bool found = false;
    int searchId = -1;
    try {
        searchId = std::stoi(searchTerm);
    } catch (...) {
    }

    for (const auto* user : users) {
        if ((searchId != -1 && user->getId() == searchId) ||
            user->getName().find(searchTerm) != std::string::npos ||
            user->getAddress().find(searchTerm) != std::string::npos ||
            user->getPhoneNumber().find(searchTerm) != std::string::npos)
        {
            if (!found) {
                std::cout << "\n--- Pengguna Ditemukan ---\n";
            }
            user->displayDetails();
            std::cout << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "Tidak ada pengguna yang cocok dengan '" << searchTerm << "'.\n";
    }
}

void Store::createUser() {
    std::cout << "\n--- Buat Pengguna Baru (Admin) ---\n";
    std::cout << "Pilih tipe (1: Buyer, 2: Seller): ";
    int type;
    std::cin >> type;
    
    if (std::cin.fail() || (type != 1 && type != 2)) {
        clearInputBuffer();
        std::cout << "Tipe tidak valid.\n";
        return;
    }

    std::string name, pass, address, phone;
    double initialBalance;
    
    std::cout << "Nama Pengguna: "; std::cin >> name;
    std::cout << "Password Sementara: "; std::cin >> pass;
    std::cin.ignore();
    std::cout << "Alamat: "; std::getline(std::cin, address);
    std::cout << "No. Telp: "; std::getline(std::cin, phone);
    std::cout << "Saldo Awal Bank: "; std::cin >> initialBalance;

    if(std::cin.fail()){
        clearInputBuffer();
        std::cout << "Input saldo tidak valid.\n";
        return;
    }

    BankCustomer* newAccount = bank.addAccount(name, initialBalance);
    int nextId = users.empty() ? 1 : users.back()->getId() + 1;

    if (type == 1) {
        users.push_back(new Buyer(nextId, name, pass, address, phone, newAccount));
    } else {
        users.push_back(new Seller(nextId, name, pass, address, phone, newAccount));
    }
    std::cout << "Pengguna baru berhasil dibuat!\n";
}

void Store::removeUser() {
    std::cout << "\n--- Hapus Pengguna Berdasarkan ID ---\n";
    std::cout << "Masukkan ID pengguna yang akan dihapus: ";
    int idToRemove;
    std::cin >> idToRemove;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ID tidak valid.\n";
        return;
    }

    auto it = std::find_if(users.begin(), users.end(), 
        [idToRemove](const Buyer* user) {
            return user->getId() == idToRemove;
        });

    if (it != users.end()) {
        int bankAccountId = (*it)->getBankAccountId();
        
        delete *it;
        users.erase(it);

        bank.removeAccountById(bankAccountId);

        std::cout << "Pengguna dengan ID " << idToRemove << " dan akun bank terkait telah dihapus.\n";
    } else {
        std::cout << "Pengguna dengan ID " << idToRemove << " tidak ditemukan.\n";
    }
}