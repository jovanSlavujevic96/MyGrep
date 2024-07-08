cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDAR 17)

project(my_grep)

add_executable(my_grep main.cpp)

find_package (Threads REQUIRED)
target_link_libraries(my_grep Threads::Threads)
