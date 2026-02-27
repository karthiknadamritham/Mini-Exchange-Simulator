#pragma once
#include "Order.h"
#include "Trade.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <iostream>

class OrderBook {
private:
    std::priority_queue<OrderPtr, std::vector<OrderPtr>, BuyComparator> buy_orders;
    std::priority_queue<OrderPtr, std::vector<OrderPtr>, SellComparator> sell_orders;
    
    // Quick lookup for orders by ID
    std::unordered_map<uint64_t, OrderPtr> order_map;
    
    uint64_t next_trade_id = 1;
    mutable std::mutex book_mutex;
    std::vector<Trade> trades;

    void match();

public:
    OrderBook() = default;
    
    // Delete copy and move
    OrderBook(const OrderBook&) = delete;
    OrderBook& operator=(const OrderBook&) = delete;

    void add_order(OrderPtr order);
    void cancel_order(uint64_t order_id);
    
    const std::vector<Trade>& get_trades() const { return trades; }
    
    void print_stats() const;
    size_t get_buy_book_size() const { return buy_orders.size(); }
    size_t get_sell_book_size() const { return sell_orders.size(); }
};
