cmake_minimum_required(VERSION 3.14)
project(MyHighsApp)

set(CMAKE_CXX_STANDARD 17)

# Use global HiGHS includes
include_directories(/usr/local/include/highs)

# Use global HiGHS library path
link_directories(/usr/local/lib)

add_executable(my_solver graph.cpp)

# Link with highs library
target_link_libraries(my_solver highs)
