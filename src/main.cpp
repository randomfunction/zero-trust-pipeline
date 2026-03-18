#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <random>
#include <iomanip>
#include "order_book.h"

void runTests() {
    OrderBook book;
    
    // Bounds check
    bool ok = book.addOrder(999, 1000000, 10, true);
    assert(!ok);

    // Simple match
    book.addOrder(1, 10050, 10, true);   
    book.addOrder(2, 10000, 5, false);   
    assert(book.getTrades() == 1);

    // Cancel order
    book.addOrder(3, 10020, 10, true);
    book.cancelOrder(3); 
    
    book.addOrder(4, 10020, 10, false); 
    assert(book.getTrades() == 2); 

    // Partial fill (buy side larger)
    book.addOrder(5, 10100, 20, true); 
    assert(book.getTrades() == 3); // Matches with remaining 5 from order 4

    book.addOrder(6, 10090, 10, false);
    assert(book.getTrades() == 4); // Matches 10 from order 5
    
    // Fill remaining of order 5
    book.addOrder(7, 10095, 10, false);
    assert(book.getTrades() == 5); // Matches remaining 5 from order 5

    // Order crossing multiple price levels
    book.addOrder(8, 10200, 10, false);
    book.addOrder(9, 10210, 10, false);
    book.addOrder(10, 10220, 25, true); 
    // Matches remaining 5 from order 7 (Trade 6)
    // Matches 10 from order 8 (Trade 7)
    // Matches 10 from order 9 (Trade 8)
    assert(book.getTrades() == 8);
    
    std::cout << "Unit Tests Passed.\n";
}

struct TestOrder {
    OrderId id;
    Price price;
    Quantity quantity;
    bool is_buy;
};

void runBenchmarks() {
    constexpr size_t NUM_ORDERS = 1000000;
    std::vector<TestOrder> orders(NUM_ORDERS);
    
    std::mt19937 gen(42); 
    std::uniform_int_distribution<Price> price_dist(90000, 110000); 
    std::uniform_int_distribution<Quantity> qty_dist(1, 100);
    std::uniform_int_distribution<int> side_dist(0, 1);
    
    for (size_t i = 0; i < NUM_ORDERS; ++i) {
        orders[i] = {
            static_cast<OrderId>(i + 1),
            price_dist(gen),
            qty_dist(gen),
            side_dist(gen) == 1
        };
    }
    
    OrderBook book;
    
    std::cout << "Starting benchmark with " << NUM_ORDERS << " orders...\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < NUM_ORDERS; ++i) {
        book.addOrder(orders[i].id, orders[i].price, orders[i].quantity, orders[i].is_buy);
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end_time - start_time;
    
    double duration = diff.count();
    double throughput = NUM_ORDERS / duration;
    
    std::cout << "Benchmark completed.\n";
    std::cout << "Total time: " << std::fixed << std::setprecision(4) << duration << " seconds\n";
    std::cout << "Throughput: " << std::fixed << std::setprecision(0) << throughput << " orders/sec\n";
    std::cout << "Trades Executed: " << book.getTrades() << "\n";
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--test") {
            runTests();
            return 0;
        } else if (arg == "--benchmark") {
            runBenchmarks();
            return 0;
        } else if (arg == "--all") {
            runTests();
            runBenchmarks();
            return 0;
        }
    }
    
    std::cout << "Trading Engine Active. Waiting for orders...\n";
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
