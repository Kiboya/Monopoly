/**
 * @file gamecore.h
 * @brief This file contains the declaration of the GameCore class, which handles the game core functionality.
 * 
 * The GameCore class is responsible for managing the game flow and executing each player's turn.
 * It contains a shared pointer to the BoardManager class, which handles the game board, and an integer to keep track of consecutive doubles rolled.
 * 
 */
#pragma once

#ifndef GAMECORE_H
#define GAMECORE_H


#include "managers.h"

class GameCore
{
    private:
        std::shared_ptr<BoardManager> boardManager; /** A shared pointer to the BoardManager class. */
        int consecutiveDoubles = 0; /** An integer to keep track of consecutive doubles rolled. */
    public:
        /**
         * @brief Constructs a new GameCore object.
         * 
         * @param spaces A vector of shared pointers to Space objects representing the game board spaces.
         * @param communityChestCards A vector of unique pointers to CommunityChestCard objects representing the community chest cards.
         * @param chanceCards A vector of unique pointers to ChanceCard objects representing the chance cards.
         */
        GameCore(std::vector<std::shared_ptr<Space>> spaces, std::vector<std::unique_ptr<CommunityChestCard>> communityChestCards, std::vector<std::unique_ptr<ChanceCard>> chanceCards);

        /**
         * @brief Destroys the GameCore object.
         */
        ~GameCore();

        /**
         * @brief Starts the game.
         * 
         * This function initializes the game board and starts the game loop.
         */
        void startGame();

        /**
         * @brief Executes a player's turn.
         * 
         * This function handles the logic for executing a player's turn, including rolling the dice, moving the player, and executing the corresponding action.
         */
        void playTurn();
};

#endif // GAMECORE_H