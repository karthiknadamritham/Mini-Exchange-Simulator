#include "OrderBook.h"
#include <chrono>
#include <algorithm>

void OrderBook::add_order(OrderPtr order) {
    std::lock_guard<std::mutex> lock(book_mutex);
    order_map[order->order_id] = order;

    if (order->type == OrderType::MARKET) {
        if (order->side == OrderSide::BUY) {
            order->price = UINT64_MAX; // Aggressive price for Market buy
            buy_orders.push(order);
        } else {
            order->price = 0; // Aggressive price for Market sell
            sell_orders.push(order);
        }
    } else {
        if (order->side == OrderSide::BUY) {
            buy_orders.push(order);
        } else {
            sell_orders.push(order);
        }
    }

    match();
}

void OrderBook::cancel_order(uint64_t order_id) {
    std::lock_guard<std::mutex> lock(book_mutex);
    auto it = order_map.find(order_id);
    if (it != order_map.end()) {
        it->second->is_canceled = true;
    }
}

void OrderBook::match() {
    while (!buy_orders.empty() && !sell_orders.empty()) {
        OrderPtr best_buy = buy_orders.top();
        OrderPtr best_sell = sell_orders.top();

        // Lazy cancellation cleanup
        if (best_buy->is_canceled) {
            buy_orders.pop();
            continue;
        }
        if (best_sell->is_canceled) {
            sell_orders.pop();
            continue;
        }

        if (best_buy->price >= best_sell->price) {
            // Match found!
            uint32_t trade_qty = std::min(best_buy->quantity, best_sell->quantity);
            
            // Trading price policy (maker price)
            uint64_t trade_price = (best_buy->timestamp <= best_sell->timestamp) ? best_buy->price : best_sell->price;

            // Handle pure market orders matching each other (edge case) or matching limits
            if (best_buy->type == OrderType::MARKET && best_sell->type != OrderType::MARKET) {
                trade_price = best_sell->price;
            } else if (best_sell->type == OrderType::MARKET && best_buy->type != OrderType::MARKET) {
                trade_price = best_buy->price;
            } else if (best_buy->type == OrderType::MARKET && best_sell->type == OrderType::MARKET) {
                trade_price = 0; // Or last traded price, simpler to just use 0 here.
            }

            uint64_t now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count();

            trades.push_back({
                next_trade_id++,
                best_buy->order_id,
                best_sell->order_id,
                trade_price,
                trade_qty,
                now
            });

            best_buy->quantity -= trade_qty;
            best_sell->quantity -= trade_qty;

            if (best_buy->quantity == 0) {
                buy_orders.pop();
            }
            if (best_sell->quantity == 0) {
                sell_orders.pop();
            }
        } else {
            // Prices don't cross
            break;
        }
    }
}

void OrderBook::print_stats() const {
    std::lock_guard<std::mutex> lock(book_mutex);
    std::cout << "--- Order Book Stats ---\n";
    std::cout << "Trades executed: " << trades.size() << "\n";
    std::cout << "Active buy orders: " << buy_orders.size() << "\n";
    std::cout << "Active sell orders: " << sell_orders.size() << "\n";
    std::cout << "------------------------\n";
}
