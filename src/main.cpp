#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <string>
#include "order_book.h"

void runTests() {
    OrderBook book;
    
    bool ok = book.addOrder(999, 1000000, 10, true);
    assert(!ok);

    book.addOrder(1, 10050, 10, true);   
    book.addOrder(2, 10000, 5, false);   
    assert(book.getTrades() == 1);

    book.addOrder(3, 10020, 10, true);
    book.cancelOrder(3); 
    
    book.addOrder(4, 10020, 10, false); 
    assert(book.getTrades() == 2); 
    
    std::cout << "Unit Tests Passed.\n";
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--test") {
        runTests();
        return 0;
    }
    
    std::cout << "Trading Engine Active. Waiting for orders...\n";
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
