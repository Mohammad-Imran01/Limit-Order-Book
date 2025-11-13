cmake_minimum_required(VERSION 3.29.0)

set(TEST_PROJ_NAME LimitOrderBookTests)

set(Sources
    LimitOrderBookTests.cpp
    # ExampleOrdersTests.cpp
    ./../constants/common.hpp
)

add_executable(${TEST_PROJ_NAME} ${Sources})
target_link_libraries(${TEST_PROJ_NAME} PUBLIC
    gtest_main
    LimitOrderBook_lib
)

add_test(
    NAME ${TEST_PROJ_NAME}
    COMMAND ${TEST_PROJ_NAME}
)