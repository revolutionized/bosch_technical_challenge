cmake_minimum_required(VERSION 3.31)
project(bosch_technical_challenge)

# Always use the latest and greatest to show off how up-to-date I am
set(CMAKE_CXX_STANDARD 23)

# Include google test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_library(bosch_technical_challenge INTERFACE
        src/Queue.tpp
        src/Queue.h
)
target_include_directories(bosch_technical_challenge INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tests
        tests/timeouts.cpp
)
target_link_libraries(tests bosch_technical_challenge gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
