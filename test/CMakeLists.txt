include(FetchContent)

FetchContent_Declare( googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(grep_test test.cpp)
target_include_directories(grep_test PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(grep_test my_grep_api)
target_link_libraries(grep_test
    GTest::gtest_main
    GTest::gmock_main
    )

include(GoogleTest)
gtest_discover_tests(grep_test)
