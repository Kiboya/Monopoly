/**
 * @file iomanager.h
 * @brief This file contains the declaration of input/output utility functions.
 * 
 */

#pragma once

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <iostream>
#include <limits>
#include <sstream>
#include "spdlog/spdlog.h"

/**
 * @brief Prompts the user to press Enter.
 *
 * @param message The message to display before prompting.
 */
void getEnter(std::string message);

/**
 * @brief Prompts the user to enter a number within a specified range.
 *
 * @param message The message to display before prompting.
 * @param min The minimum value allowed.
 * @param max The maximum value allowed.
 * @return The number entered by the user.
 */
int getNumber(std::string message, int min, int max);

/**
 * @brief Prompts the user to enter a string.
 *
 * @param message The message to display before prompting.
 * @param validStrings A vector of valid strings. If provided, the user's input must match one of the strings in the vector.
 * @return The string entered by the user.
 */
std::string getString(std::string message, const std::vector<std::string>& validStrings = {});

/**
 * @brief Prompts the user to enter 'y' or 'n'.
 *
 * @param message The message to display before prompting.
 * @return 'y' if the user enters 'y', 'n' if the user enters 'n'.
 */
char getYesNo(std::string message);

/**
 * @brief Logs the content of an object to the console using the specified log level.
 *
 * @tparam T The type of the object to log.
 * @param object The object to log.
 * @param level The log level (default: spdlog::level::info).
 */
template <typename T>
void logObject(const T& object, spdlog::level::level_enum level = spdlog::level::info)
{
    std::ostringstream oss;
    oss << object;
    std::istringstream iss(oss.str());
    std::string line;
    while (std::getline(iss, line))
    {
        spdlog::log(level, line);
    }
}

#endif // INPUT_MANAGER_H