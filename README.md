# 📈 Mini Exchange Simulator (Order Matching Engine)

A high-performance **Real-Time Order Matching Engine** and **Exchange Simulator** built in Modern C++. 

This project simulates the core mechanics of a financial exchange, accepting Limit and Market orders, maintaining an order book, and executing trades with ultra-low latency. It is designed to act as a backend system capable of sustaining high throughput for simulated algorithmic trading.

## ✨ Features

- **Ultra-Fast Order Matching:** Custom memory-optimized Order Book implementation designed for speed.
- **Multi-Threaded Architecture:** Utilizes a lock-free or highly concurrent queue system to separate order ingestion from the matching engine loop.
- **Support for Core Order Types:**
  - **Limit Orders:** Executes at a specified price or better.
  - **Market Orders:** Executes immediately against the best available price.
- **Time/Price Priority:** Strict adherence to standard financial matching rules (FIFO).
- **Comprehensive Benchmarking:** Built-in latency and throughput benchmarking (e.g., tests 100,000+ generated orders).
- **Trade Reconciliation & P&L:** Built-in statistics and logging to track processed trades and engine performance.

## 🛠️ Technologies & Tools

- **Language:** C++11 / C++14 or higher
- **Build System:** CMake
- **Concurrency:** `<thread>`, `<mutex>`, `<condition_variable>`
- **Data Structures:** Custom-optimized standard library structures (Vectors, Maps/Unordered Maps)

## 🚀 Getting Started

### Prerequisites

- A modern C++ Compiler (GCC, Clang, or MSVC)
- CMake (version 3.10+)

### Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/karthiknadamritham/Mini-Exchange-Simulator.git
   cd Mini-Exchange-Simulator
   ```

2. Generate the build files using CMake:
   ```bash
   cmake -S . -B build
   ```

3. Build the executable:
   ```bash
   cmake --build build --config Release
   ```

### Running the Engine Benchmark

Once compiled, you can run the simulator to run the benchmark:

**On Windows:**
```bash
.\build\Release\Engine.exe
```
*(Or if not using multip-configuration generators, simply `.\build\Engine.exe`)*

**On Linux/macOS:**
```bash
./build/Engine
```

The engine will automatically generate a set of random orders (e.g., 100,000 orders) and provide a benchmark of throughput (Orders/sec) and average latency per order.

## 📊 Example Output

```text
Starting High-Performance Order Matching Engine Benchmark...
Pre-generating 100000 orders to isolate engine latency...
Submitting orders to multi-threaded engine queue...

=== Benchmark Results ===
Total Orders Processed: 100000
Total Execution Time: <X> ms
Throughput: <Y> orders/sec
Average Latency per order: <Z> microseconds
```

## 🤝 Contributing
Contributions, issues, and feature requests are welcome! Feel free to check the issues page.

## 📝 License
This project is open-source and available under the [MIT License](LICENSE).
