

/**
 * @file iomanager.cc
 * @brief Implementation of the I/O Manager class.
 */

#include "iomanager.h"
#include <random>

#ifdef TEST
    /**
     * @brief Simulates user input for testing purposes.
     * @param message The message to display to the user.
     */
    void getEnter(std::string message){
        spdlog::info(message);
    }

    /**
     * @brief Simulates user input for testing purposes and returns a random number within the specified range.
     * @param message The message to display to the user.
     * @param min The minimum value of the random number.
     * @param max The maximum value of the random number.
     * @return A random number within the specified range.
     */
    int getNumber(std::string message, int min, int max){
        spdlog::info(message);
        std::uniform_int_distribution<int> distribution(min, max);
        std::default_random_engine generator = std::default_random_engine(std::random_device()());
        int randomNum = distribution(generator);
        spdlog::debug("Value returned: {0}", randomNum);
        return randomNum;
    }

    /**
     * @brief Simulates user input for testing purposes and returns a random string from the given vector of valid strings.
     * @param message The message to display to the user.
     * @param validStrings The vector of valid strings.
     * @return A random string from the vector of valid strings.
     */
    std::string getString(std::string message, const std::vector<std::string>& validStrings){
        spdlog::info(message);
        if (message.find("name") != std::string::npos) {
            static std::vector<std::string> names = {"Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace", "Henry"};
            static int index = 0;
            std::string name = names[index++ % names.size()];
            spdlog::debug("Value returned: {0}", name);
            return name;
        } else if (!validStrings.empty()) {
            std::uniform_int_distribution<int> distribution(0, validStrings.size() - 1);
            std::default_random_engine generator = std::default_random_engine(std::random_device()());
            int randomIndex = distribution(generator);
            spdlog::debug("Value returned: {0}", validStrings[randomIndex]);
            return validStrings[randomIndex];
        }
        spdlog::debug("Value returned: testString");
        return "testString";
    }

    /**
     * @brief Simulates user input for testing purposes and returns 'y' or 'n' randomly.
     * @param message The message to display to the user.
     * @return 'y' or 'n' randomly.
     */
    char getYesNo(std::string message){
        spdlog::info(message);
        std::uniform_int_distribution<int> distribution(0, 1);
        std::default_random_engine generator = std::default_random_engine(std::random_device()());
        int randomNum = distribution(generator);
        spdlog::debug("Value returned: {0}", randomNum == 0 ? 'y' : 'n');
        return randomNum == 0 ? 'y' : 'n';
    }
#else
    /**
     * @brief Gets user input from the console.
     * @param message The message to display to the user.
     */
    void getEnter(std::string message){
        spdlog::info(message);
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) {
            spdlog::warn("Invalid input. Ignoring entered value.");
        }
    }

    /**
     * @brief Gets a number from the user within the specified range.
     * @param message The message to display to the user.
     * @param min The minimum value of the number.
     * @param max The maximum value of the number.
     * @return The number entered by the user.
     */
    int getNumber(std::string message, int min, int max){
        int number;
        std::string input;
        do
        {
            spdlog::info(message);
            std::getline(std::cin, input);
            try {
                number = std::stoi(input);
                if (number < min || number > max)
                {
                    spdlog::error("Invalid input. Please enter a number between {0} and {1}.", min, max);
                }
            } catch (const std::invalid_argument& e) {
                spdlog::error("Invalid input. Please enter a number.");
            }
        } while (number < min || number > max);
        spdlog::debug("Value returned: {0}", number);
        return number;
    }

    /**
     * @brief Gets a string from the user.
     * @param message The message to display to the user.
     * @param validStrings The vector of valid strings. If empty, any non-empty string is considered valid.
     * @return The string entered by the user.
     */
    std::string getString(std::string message, const std::vector<std::string>& validStrings) {
        std::string str;
        bool isValid = false;
        spdlog::info(message);
        do {
            std::getline(std::cin, str);
            if (str.empty()) {
                spdlog::error("Invalid input. Please enter a non-empty string.");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                spdlog::info(message);
            } else if (!validStrings.empty()) {
                for (const std::string& validString : validStrings) {
                    if (strcasecmp(str.c_str(), validString.c_str()) == 0) {
                        isValid = true;
                        break;
                    }
                }
                if (!isValid) {
                    spdlog::error("Invalid input. Please enter one of the valid strings.");
                    spdlog::info("Valid strings:");
                    for (const std::string& validString : validStrings) {
                        spdlog::info(validString);
                    }
                    spdlog::info(message);
                }
            }
        } while (str.empty() || (!validStrings.empty() && !isValid));
        spdlog::debug("Value returned: {0}", str);
        return str;
    }

    /**
     * @brief Gets a 'y' or 'n' answer from the user.
     * @param message The message to display to the user.
     * @return 'y' or 'n' entered by the user.
     */
    char getYesNo(std::string message){
        std::string input;
        char answer;
        do {
            spdlog::info(message);
            std::getline(std::cin, input);
            if (!input.empty()) {
                answer = input[0];
            }
            if (answer != 'y' && answer != 'n') {
                spdlog::error("Invalid input. Please enter 'y' or 'n'.");
            }
        } while (answer != 'y' && answer != 'n');
        spdlog::debug("Value returned: {0}", answer);
        return answer;
    }
#endif