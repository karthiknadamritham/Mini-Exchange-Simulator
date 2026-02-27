#include "Engine.h"

Engine::Engine() {}

Engine::~Engine() {
    stop();
}

void Engine::start() {
    if (running) return;
    running = true;
    worker = std::thread(&Engine::process_orders, this);
}

void Engine::stop() {
    if (!running) return;
    running = false;
    cv.notify_all();
    if (worker.joinable()) {
        worker.join();
    }
}

void Engine::submit_order(OrderPtr order) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        incoming_orders.push(order);
    }
    cv.notify_one();
}

void Engine::cancel_order(uint64_t order_id) {
    // Forward direct to thread safe order book map
    book.cancel_order(order_id);
}

void Engine::process_orders() {
    while (true) {
        OrderPtr order = nullptr;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [this] { return !incoming_orders.empty() || !running; });
            
            if (incoming_orders.empty() && !running) {
                break;
            }
            
            if (!incoming_orders.empty()) {
                order = incoming_orders.front();
                incoming_orders.pop();
            }
        }

        if (order) {
            book.add_order(order);
        }
    }
}
