#pragma once
#include <cstdint>
#include <chrono>
#include <memory>

enum class OrderType { LIMIT, MARKET };
enum class OrderSide { BUY, SELL };

struct Order {
    uint64_t order_id;
    uint64_t price;     // Using integer (e.g. cents) for precision
    uint32_t quantity;
    OrderSide side;
    OrderType type;
    uint64_t timestamp; // nanoseconds since epoch
    bool is_canceled{false};

    Order(uint64_t id, uint64_t p, uint32_t q, OrderSide s, OrderType t)
        : order_id(id), price(p), quantity(q), side(s), type(t), is_canceled(false) {
        timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }
};

using OrderPtr = std::shared_ptr<Order>;

// Max heap: Highest price first. If tied, earliest time.
struct BuyComparator {
    bool operator()(const OrderPtr& a, const OrderPtr& b) const {
        if (a->price != b->price) return a->price < b->price;
        return a->timestamp > b->timestamp; 
    }
};

// Min heap: Lowest price first. If tied, earliest time.
struct SellComparator {
    bool operator()(const OrderPtr& a, const OrderPtr& b) const {
        if (a->price != b->price) return a->price > b->price;
        return a->timestamp > b->timestamp;
    }
};
