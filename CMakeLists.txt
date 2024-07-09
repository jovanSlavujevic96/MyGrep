cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDAR 17)

project(my_grep)

add_library(my_grep_api STATIC
    "my_grep_api/IActionExecutor.h"
    "my_grep_api/Printer.h"
    "my_grep_api/MyGrep.h"
    "my_grep_api/MyGrep.cpp"
    )
find_package (Threads REQUIRED)
target_link_libraries(my_grep_api PUBLIC Threads::Threads)

add_executable(my_grep main.cpp)
target_link_libraries(my_grep my_grep_api)

add_subdirectory(test)

