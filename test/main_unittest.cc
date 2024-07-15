/**
 * @file main_unittest.cc
 * @brief Main file for the unit tests
 * 
 * This file contains the main function for running the unit tests. It sets up the logger, initializes the Google Test framework, and runs all the tests.
 */

#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h" 
#include <chrono>
#include <sstream>
#include <iomanip>


/**
 * @brief Main function for running the unit tests
 * 
 * This function sets up the logger, initializes the Google Test framework, and runs all the tests.
 * 
 * @param argc Number of command-line arguments
 * @param argv Command-line arguments
 * @return int 0 if all tests pass, 1 otherwise
 */
int main(int argc, char **argv) {
    // For testing purposes, we will log to a file instead of the console
    try
    {    
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream filename;
        filename << "../logs/logs_" << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S") << ".txt";
        auto file_logger = spdlog::basic_logger_mt("file_log", filename.str());
        file_logger->flush_on(spdlog::level::debug);
        spdlog::set_default_logger(file_logger);
        spdlog::set_level(spdlog::level::debug);
        spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }

    //Test setup
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::GTEST_FLAG(filter) = "Deck.Shuffle";
    //::testing::GTEST_FLAG(repeat) = 100; 
    ::testing::GTEST_FLAG(break_on_failure) = true;
    return RUN_ALL_TESTS();
}