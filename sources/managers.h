/**
 * @file managers.h
 * @brief This file contains the declaration of classes related to managing players, spaces, and cards in a game.
 * 
 * This file defines the following classes:
 * - Dice: Represents a dice used for rolling in the game.
 * - PlayerManager: Manages the players in the game.
 * - BoardManager: Manages the board, properties, and cards in the game.
 * 
 */

#pragma once

#ifndef MANAGERS_H
#define MANAGERS_H

#include "spaces.h"
#include "player.h"
#include "cards.h"
#include <vector>
#include <optional>
#include <random>
#include <memory>
#include <map>

// Forward declaration
class Space; 
class Property;
enum class Color;
class CommunityChestCard; 
class ChanceCard; 

/**
 * @brief Represents a dice used for rolling in the game.
 */
class Dice
{
    private:
        int value; /** The current value of the dice. */
        std::default_random_engine generator; /** The random number generator used for rolling the dice. */
        std::uniform_int_distribution<int> distribution; /** The distribution of possible dice values. */
    public:
        /**
         * @brief Constructs a new Dice object.
         */
        Dice();
        
        /**
         * @brief Destroys the Dice object.
         */
        ~Dice();
        
        /**
         * @brief Rolls the dice and returns the result.
         * @return The value obtained from rolling the dice.
         */
        int roll();
};

/**
 * @brief Manages the players in the game.
 */
class PlayerManager
{
    private:
        std::vector<std::shared_ptr<Player>> players; /** The list of players in the game. */
        std::shared_ptr<Player> currentPlayer; /** The current player. */
    public:
        /**
         * @brief Constructs a new PlayerManager object.
         */
        PlayerManager();
        
        /**
         * @brief Destroys the PlayerManager object.
         */
        ~PlayerManager();
        
        /**
         * @brief Adds a player to the game.
         * @param player The player to add.
         */
        void addPlayer(std::shared_ptr<Player> player); 
        
        /**
         * @brief Removes a player from the game.
         * @param player The player to remove.
         */
        void removePlayer(std::shared_ptr<Player> player);
        
        /**
         * @brief Sets the current player.
         * @param player The player to set as the current player.
         */
        void setCurrentPlayer(std::shared_ptr<Player> player);
        
        /**
         * @brief Gets a player by index.
         * @param index The index of the player.
         * @return The player at the specified index.
         */
        std::shared_ptr<Player> getPlayer(int index);
        
        /**
         * @brief Gets the number of players in the game.
         * @return The number of players.
         */
        int getNbPlayers();
        
        /**
         * @brief Gets the current player.
         * @return The current player.
         */
        std::shared_ptr<Player> getCurrentPlayer();
        
        /**
         * @brief Sets the next player as the current player.
         */
        void setNextPlayer();
        
        /**
         * @brief Transfers money from one player to another.
         * @param player1 The player to transfer money from.
         * @param player2 The player to transfer money to.
         * @param amount The amount of money to transfer.
         * 
         * If either player is nullptr, it represents the bank.
         */
        void transferMoneyFromTo(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, int amount);
};

/**
 * @brief Manages the board, properties, and cards in the game.
 */
class BoardManager : public std::enable_shared_from_this<BoardManager>
{
    private:
        std::vector<std::shared_ptr<Space>> board; /** The board containing all the spaces in the game. */
        std::map<Color, std::vector<std::shared_ptr<Property>>> properties; /** The properties grouped by color. */
        std::vector<std::unique_ptr<ChanceCard>> chanceDeck; /** The deck of chance cards. */
        bool chanceGOJFCTaken = false; /** Indicates whether the Get Out of Jail Free card has been taken from the chance deck. */
        std::vector<std::unique_ptr<CommunityChestCard>> communityChestDeck; /** The deck of community chest cards. */
        bool communityChestGOJFCTaken = false; /** Indicates whether the Get Out of Jail Free card has been taken from the community chest deck. */
        std::shared_ptr<PlayerManager> playerManager; /** The player manager. */
        Dice dice1, dice2; /** The two dice used for rolling. */
        std::pair<int, int> currentDicesValue; /** The current values of the dice. */
    public:
        /**
         * @brief Constructs a new BoardManager object.
         * @param _board The board containing all the spaces.
         * @param _communityChestDeck The deck of community chest cards.
         * @param _chanceDeck The deck of chance cards.
         */
        BoardManager(std::vector<std::shared_ptr<Space>> _board, std::vector<std::unique_ptr<CommunityChestCard>> _communityChestDeck, std::vector<std::unique_ptr<ChanceCard>> _chanceDeck);
        
        /**
         * @brief Destroys the BoardManager object.
         */
        ~BoardManager();
        
        /**
         * @brief Gets the player manager.
         * @return The player manager.
         */
        std::shared_ptr<PlayerManager> getPlayerManager();   
        
        /**
         * @brief Gets the board.
         * @return The board.
         */
        std::vector<std::shared_ptr<Space>> getBoard();
        
        /**
         * @brief Gets the groups of properties owned by a player.
         * @param player The player.
         * @return The groups of properties owned by the player.
         */
        std::vector<std::shared_ptr<Property>> getOwnedGroups(std::shared_ptr<Player> player);
        
        /**
         * @brief Gets the current values of the dice.
         * @return The current values of the dice.
         */
        std::pair<int, int> getCurrentDicesValue();
        
        /**
         * @brief Rolls the dice.
         */
        void rollDice();
        
        /**
         * @brief Draws a chance card.
         */
        void drawChanceCard();
        
        /**
         * @brief Sets whether the Get Out of Jail Free card has been taken from the chance deck.
         * @param taken Whether the card has been taken.
         */
        void setChanceGOJFCTaken(bool taken);
        
        /**
         * @brief Draws a community chest card.
         */
        void drawCommunityChestCard();
        
        /**
         * @brief Sets whether the Get Out of Jail Free card has been taken from the community chest deck.
         * @param taken Whether the card has been taken.
         */
        void setCommunityChestGOJFCTaken(bool taken);
        
        /**
         * @brief Moves the current player on the board.
         * @param diceValue The value obtained from rolling the dice.
         */
        void movePlayer(int diceValue);
        
        /**
         * @brief Assigns ownership of a space to a player.
         * @param player The player.
         * @param space The space to assign ownership to.
         */
        void affectOwnership(std::shared_ptr<Player> player, std::shared_ptr<Space> space);
        
        /**
         * @brief Builds houses/hotels on properties.
         * @param properties The properties to build on.
         * @param player The player who owns the properties.
         * @param attempts The number of attempts made to build.
         */
        void buildOnProperties(std::vector<std::shared_ptr<Property>> properties, std::shared_ptr<Player> player, int attempts = 0);
        
        /**
         * @brief Handles the current space the player is on.
         */
        void handleSpace();
};

#endif // MANAGERS_H