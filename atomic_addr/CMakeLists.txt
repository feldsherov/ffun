cmake_minimum_required(VERSION 3.9)

add_executable(atomic_addr atomic_addr.cpp)
target_link_libraries(atomic_addr gtest gtest_main)
