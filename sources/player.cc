/**
 * @file player.cc
 * @brief Implementation file for the Player class
 * 
 * This file contains the implementation of the Player class, which represents a player in a game.
 * The Player class provides methods to get and set the player's position, money, remaining turns in jail,
 * number of stations and utilities owned, and whether the player has a chance or community chest get out of jail free card.
 * 
 */

#include "player.h"
#include <iostream>

Player::Player(std::string _name) : name(_name)
{
}

Player::~Player()
{
}

int Player::getPosition() const
{
    return position;
}

void Player::setPosition(int _position)
{
    position = _position;
}

int Player::getMoney() const
{
    return money;
}

void Player::setMoney(int _money)
{
    money = _money;
}

int Player::getRemainingTurnsInJail() const
{
    return remainingTurnsInJail;
}

void Player::setRemainingTurnsInJail(int _remainingTurnsInJail)
{
    remainingTurnsInJail = _remainingTurnsInJail;
}

int Player::getNbStationsOwned() const
{
    return nbStationsOwned;
}

void Player::setNbStationsOwned(int _nbStationsOwned)
{
    nbStationsOwned = _nbStationsOwned;
}

bool Player::getHasChanceGOJFC() const
{
    return hasChanceGOJFC;
}

void Player::setHasChanceGOJFC(bool _hasChanceGOJFC)
{
    hasChanceGOJFC = _hasChanceGOJFC;
}

bool Player::getHasCommunityChestGOJFC() const
{
    return hasCommunityChestGOJFC;
}

void Player::setHasCommunityChestGOJFC(bool _hasCommunityChestGOJFC)
{
    hasCommunityChestGOJFC = _hasCommunityChestGOJFC;
}

int Player::getNbUtilitiesOwned() const
{
    return nbUtilitiesOwned;
}

void Player::setNbUtilitiesOwned(int _nbUtilitiesOwned)
{
    nbUtilitiesOwned = _nbUtilitiesOwned;
}

std::string Player::getName() const
{
    return name;
}

