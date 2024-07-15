/**
 * @file player.h
 * @brief This file contains the declaration of the Player class.
 * 
 * The Player class represents a player in a game. It stores information such as the player's name, money, position, 
 * number of stations and utilities owned, remaining turns in jail, and whether the player has a Get Out of Jail Free Card.
 * 
 */
#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
private:
    std::string name;      /**< The name of the player. */
    int money = 1500;            /**< The amount of money the player has. */
    int position = 0;            /**< The position of the player on the game board. */
    int nbStationsOwned = 0;     /**< The number of stations owned by the player. */
    int nbUtilitiesOwned = 0;    /**< The number of utilities owned by the player. */
    int remainingTurnsInJail = 0;/**< The remaining turns the player has to spend in jail. */
    bool hasChanceGOJFC = false; /**< Indicates whether the player has a Get Out of Jail Free Card from the Chance deck. */
    bool hasCommunityChestGOJFC = false; /**< Indicates whether the player has a Get Out of Jail Free Card from the Community Chest deck. */
public:
    /**
     * @brief Constructs a new Player object with the given name.
     * 
     * @param _name The name of the player.
     */
    Player(std::string _name);

    /**
     * @brief Destroys the Player object.
     */
    ~Player();

    /**
     * @brief Gets the position of the player on the game board.
     * 
     * @return int The position of the player.
     */
    int getPosition() const;

    /**
     * @brief Sets the position of the player on the game board.
     * 
     * @param _position The position to set.
     */
    void setPosition(int _position);

    /**
     * @brief Gets the amount of money the player has.
     * 
     * @return int The amount of money.
     */
    int getMoney() const;

    /**
     * @brief Sets the amount of money the player has.
     * 
     * @param _money The amount of money to set.
     */
    void setMoney(int _money);

    /**
     * @brief Gets the remaining turns the player has to spend in jail.
     * 
     * @return int The remaining turns in jail.
     */
    int getRemainingTurnsInJail() const;

    /**
     * @brief Sets the remaining turns the player has to spend in jail.
     * 
     * @param _remainingTurnsInJail The remaining turns in jail to set.
     */
    void setRemainingTurnsInJail(int _remainingTurnsInJail);

    /**
     * @brief Checks if the player has a Get Out of Jail Free Card from the Chance deck.
     * 
     * @return bool True if the player has the card, false otherwise.
     */
    bool getHasChanceGOJFC() const;

    /**
     * @brief Sets whether the player has a Get Out of Jail Free Card from the Chance deck.
     * 
     * @param _hasChanceGOJFC True if the player has the card, false otherwise.
     */
    void setHasChanceGOJFC(bool _hasChanceGOJFC);

    /**
     * @brief Checks if the player has a Get Out of Jail Free Card from the Community Chest deck.
     * 
     * @return bool True if the player has the card, false otherwise.
     */
    bool getHasCommunityChestGOJFC() const;

    /**
     * @brief Sets whether the player has a Get Out of Jail Free Card from the Community Chest deck.
     * 
     * @param _hasCommunityChestGOJFC True if the player has the card, false otherwise.
     */
    void setHasCommunityChestGOJFC(bool _hasCommunityChestGOJFC);

    /**
     * @brief Gets the number of stations owned by the player.
     * 
     * @return int The number of stations owned.
     */
    int getNbStationsOwned() const;

    /**
     * @brief Sets the number of stations owned by the player.
     * 
     * @param _nbStationsOwned The number of stations owned to set.
     */
    void setNbStationsOwned(int _nbStationsOwned);

    /**
     * @brief Gets the number of utilities owned by the player.
     * 
     * @return int The number of utilities owned.
     */
    int getNbUtilitiesOwned() const;

    /**
     * @brief Sets the number of utilities owned by the player.
     * 
     * @param _nbUtilitiesOwned The number of utilities owned to set.
     */
    void setNbUtilitiesOwned(int _nbUtilitiesOwned);

    /**
     * @brief Gets the name of the player.
     * 
     * @return std::string The name of the player.
     */
    std::string getName() const;
};

#endif // PLAYER_H
