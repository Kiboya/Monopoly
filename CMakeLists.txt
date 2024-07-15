cmake_minimum_required(VERSION 3.5)
project(Monopoly)

include(FetchContent)

# spdlog 
FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/refs/heads/v1.x.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(spdlog)

add_library(spdlog_interface INTERFACE)
target_include_directories(spdlog_interface INTERFACE ${spdlog_SOURCE_DIR}/include)
target_link_libraries(spdlog_interface INTERFACE spdlog::spdlog_header_only)

# googletest
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

add_library(gtest_interface INTERFACE)
target_link_libraries(gtest_interface INTERFACE GTest::gtest_main)

file(GLOB_RECURSE SOURCES "sources/*.cc" "sources/*.h")
file(GLOB_RECURSE TESTS "test/*.cc")
file(GLOB_RECURSE UTILS "utils/*.cc" "utils/*.h")

# Release
add_executable(Release ${UTILS} ${SOURCES} main.cc)
target_link_libraries(Release PRIVATE spdlog_interface)
target_compile_features(Release PRIVATE cxx_std_17)
target_compile_definitions(Release PRIVATE NDEBUG)

# Debug
add_executable(Debug ${UTILS} ${SOURCES} main.cc)
target_link_libraries(Debug PRIVATE spdlog_interface)
target_compile_features(Debug PRIVATE cxx_std_17)
target_compile_definitions(Debug PRIVATE DEBUG)

# Tests
add_executable(Tests ${UTILS} ${SOURCES} ${TESTS})
target_link_libraries(Tests PRIVATE gtest_interface spdlog_interface)
target_compile_features(Tests PRIVATE cxx_std_17)
target_compile_definitions(Tests PRIVATE TEST)

