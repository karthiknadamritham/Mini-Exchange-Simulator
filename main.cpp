#include "Engine.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

int main() {
    std::cout << "Starting High-Performance Order Matching Engine Benchmark...\n";
    Engine engine;
    engine.start();

    const int NUM_ORDERS = 100000;
    
    // Setup random generation
    std::mt19937_64 rng(1337);
    std::uniform_int_distribution<uint64_t> price_dist(9000, 11000); // 90.00 to 110.00
    std::uniform_int_distribution<uint32_t> qty_dist(10, 1000);
    std::uniform_int_distribution<int> side_dist(0, 1);
    std::uniform_int_distribution<int> type_dist(0, 9); // 10% market orders, 90% limit

    std::vector<OrderPtr> orders;
    orders.reserve(NUM_ORDERS);

    std::cout << "Pre-generating " << NUM_ORDERS << " orders to isolate engine latency...\n";
    for (int i = 0; i < NUM_ORDERS; ++i) {
        OrderSide side = (side_dist(rng) == 0) ? OrderSide::BUY : OrderSide::SELL;
        OrderType type = (type_dist(rng) == 0) ? OrderType::MARKET : OrderType::LIMIT;
        uint64_t price = price_dist(rng);
        uint32_t qty = qty_dist(rng);

        orders.push_back(std::make_shared<Order>(i + 1, price, qty, side, type));
    }

    std::cout << "Submitting orders to multi-threaded engine queue...\n";

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ORDERS; ++i) {
        engine.submit_order(orders[i]);
    }

    engine.stop(); // Blocks until all pending orders in the queue are processed by book

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    double duration_ms = duration.count();
    double ops = (NUM_ORDERS / duration_ms) * 1000.0;
    double avg_latency = (duration_ms * 1000.0) / NUM_ORDERS; 

    std::cout << "\n=== Benchmark Results ===\n";
    std::cout << "Total Orders Processed: " << NUM_ORDERS << "\n";
    std::cout << "Total Execution Time: " << duration_ms << " ms\n";
    std::cout << "Throughput: " << static_cast<uint64_t>(ops) << " orders/sec\n";
    std::cout << "Average Latency per order (including queue overhead): " << avg_latency << " microseconds\n\n";

    engine.get_book().print_stats();

    return 0;
}
