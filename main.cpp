// main.cpp
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

#include "bank.h"
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "store.h" 

using namespace std;

int main() {
    Store onlineStore;
    onlineStore.run(); 

    return 0;
}