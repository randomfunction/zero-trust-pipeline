#include <iostream>
#include <vector>
#include <cassert>
#include <thread>
#include <chrono>
#include <string>
#include <cstdint>
#include <algorithm>

struct alignas(64) Order {
    double price;
    uint32_t quantity;
    bool is_buy;
};

struct alignas(64) LevelData {
    uint32_t buy_quantity = 0;
    uint32_t sell_quantity = 0;
};

constexpr size_t MAX_PRICE_LEVELS = 1000000;

class OrderBook {
    std::vector<LevelData> book;
    uint32_t trades_executed = 0;

    int max_buy_price = 0;
    int min_sell_price = static_cast<int>(MAX_PRICE_LEVELS);

public:
    OrderBook() {
        book.resize(MAX_PRICE_LEVELS);
    }

    void addOrder(const Order& order) {
        int price_index = static_cast<int>(order.price * 100);
        
        if (order.is_buy) {
            book[price_index].buy_quantity += order.quantity;
            if (price_index > max_buy_price) max_buy_price = price_index;
        } else {
            book[price_index].sell_quantity += order.quantity;
            if (price_index < min_sell_price) min_sell_price = price_index;
        }
        match();
    }

    void match() {
        while (max_buy_price >= min_sell_price) {
            while (max_buy_price >= 0 && book[max_buy_price].buy_quantity == 0) {
                max_buy_price--;
            }
            while (min_sell_price < static_cast<int>(MAX_PRICE_LEVELS) && book[min_sell_price].sell_quantity == 0) {
                min_sell_price++;
            }
            
            if (max_buy_price < min_sell_price || max_buy_price < 0 || min_sell_price >= static_cast<int>(MAX_PRICE_LEVELS)) {
                break;
            }
            
            uint32_t traded = std::min(book[max_buy_price].buy_quantity, book[min_sell_price].sell_quantity);
            book[max_buy_price].buy_quantity -= traded;
            book[min_sell_price].sell_quantity -= traded;
            trades_executed++;
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