#include <iostream>
#include <queue>
#include <vector>
#include <cassert>
#include <thread>
#include <chrono>
#include <string>

struct Order {
    double price;
    int quantity;
    bool is_buy;
};

struct CompareBuy {
    bool operator()(const Order& a, const Order& b) { return a.price < b.price; }
};

struct CompareSell {
    bool operator()(const Order& a, const Order& b) { return a.price > b.price; }
};

class OrderBook {
    std::priority_queue<Order, std::vector<Order>, CompareBuy> buys;
    std::priority_queue<Order, std::vector<Order>, CompareSell> sells;
    int trades_executed = 0;

public:
    void addOrder(const Order& order) {
        if (order.is_buy) buys.push(order);
        else sells.push(order);
        match();
    }

    void match() {
        while (!buys.empty() && !sells.empty() && buys.top().price >= sells.top().price) {
            Order b = buys.top(); buys.pop();
            Order s = sells.top(); sells.pop();
            int traded = std::min(b.quantity, s.quantity);
            
            b.quantity -= traded;
            s.quantity -= traded;
            trades_executed++;

            if (b.quantity > 0) buys.push(b);
            if (s.quantity > 0) sells.push(s);
        }
    }

    int getTrades() const { return trades_executed; }
};

void runTests() {
    OrderBook book;
    book.addOrder({100.5, 10, true});
    book.addOrder({100.0, 5, false});
    assert(book.getTrades() == 1);
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