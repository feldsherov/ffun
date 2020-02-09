cmake_minimum_required(VERSION 3.9)

project(feldsherov_fun)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")

add_subdirectory(gtest)

include(cmake/CompileOptions.cmake)
include(cmake/Logging.cmake)
include(cmake/Ccache.cmake)

add_subdirectory(atomic_addr)
add_subdirectory(splay)
add_subdirectory(short_string_optimization)
add_subdirectory(init_statement)
