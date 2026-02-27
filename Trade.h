#pragma once
#include <cstdint>

struct Trade {
    uint64_t trade_id;
    uint64_t buyer_order_id;
    uint64_t seller_order_id;
    uint64_t price;
    uint32_t quantity;
    uint64_t timestamp;
};
