/**
 * @file cards.cc
 * @brief Implementation file for the Card, CommunityChestCard, and ChanceCard classes.
 * 
 * This file contains the implementation of the Card, CommunityChestCard, and ChanceCard classes.
 * These classes are used to handle the Community Chest and Chance cards in a game.
 * 
 */

#include "cards.h"
#include "spdlog/spdlog.h"


Card::Card(std::string _description) : description(_description) {}

Card::~Card() {}

std::string Card::getDescription() const
{
    return description;
}

CommunityChestCard::CommunityChestCard(std::string _description, std::function<void(std::shared_ptr<BoardManager>)> _action) : Card(_description)  
{
    _actionFunction = _action;  
}

CommunityChestCard::~CommunityChestCard() {}

void CommunityChestCard::action(std::shared_ptr<BoardManager> board)
{
    if (_actionFunction)
    {
        spdlog::info("Community Chest Card: {0}", getDescription());
        _actionFunction(board); // Call the action function associated with the card
    }
}


std::ostream& operator<<(std::ostream& os, const CommunityChestCard& card)
{
    os << card.getDescription();
    return os;
}

ChanceCard::ChanceCard(std::string _description, std::function<void(std::shared_ptr<BoardManager> )> _action) : Card(_description) 
{
    _actionFunction = _action;
}

ChanceCard::~ChanceCard() {}

void ChanceCard::action(std::shared_ptr<BoardManager>  board)
{
    if (_actionFunction)
    {
        spdlog::info("Chance Card: {0}", getDescription());
        _actionFunction(board); // Call the action function associated with the card
    }
}

std::ostream& operator<<(std::ostream& os, const ChanceCard& card)
{
    os << card.getDescription();
    return os;
}