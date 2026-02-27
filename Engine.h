#pragma once
#include "OrderBook.h"
#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>

class Engine {
private:
    OrderBook book;
    std::queue<OrderPtr> incoming_orders; // Lock-protected queue
    std::mutex queue_mutex;
    std::condition_variable cv;
    std::atomic<bool> running{false};
    std::thread worker;

    void process_orders();

public:
    Engine();
    ~Engine();

    // Prevent copy and move
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void start();
    void stop();
    
    void submit_order(OrderPtr order);
    void cancel_order(uint64_t order_id);

    const OrderBook& get_book() const { return book; }
};
