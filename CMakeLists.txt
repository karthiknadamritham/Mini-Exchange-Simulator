cmake_minimum_required(VERSION 3.10)
project(MiniExchangeSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimize for speed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread")

add_executable(exchange_engine
    main.cpp
    OrderBook.cpp
    Engine.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(exchange_engine Threads::Threads)
