cmake_minimum_required(VERSION 3.9)

project(feldsherov_fun)

include(cmake/CompileOptions.cmake)
include(cmake/Logging.cmake)
include(cmake/Ccache.cmake)

add_subdirectory(gtest)

add_subdirectory(splay)
