#pragma once
#include <cstdint>

constexpr size_t MAX_PRICE_LEVELS = 1000000;
constexpr size_t MAX_ORDERS = 1000000;

using OrderId = uint64_t;
using Price = uint64_t;
using Quantity = uint32_t;

// Intrusive Doubly Linked List Node for individual orders
struct Order {
    OrderId id;
    Price price;
    Quantity quantity;
    bool is_buy;

    Order* prev = nullptr;
    Order* next = nullptr;
};
