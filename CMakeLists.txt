cmake_minimum_required(VERSION 3.9)

project(feldsherov_fun)

add_subdirectory(gtest)

include(cmake/CompileOptions.cmake)
include(cmake/Logging.cmake)
include(cmake/Ccache.cmake)

add_subdirectory(splay)
add_subdirectory(short_string_optimization)
