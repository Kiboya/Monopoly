/**
 * @file cards.h
 * @brief This file contains the declaration of the Card, CommunityChestCard, and ChanceCard classes.
 * 
 * The Card class represents a generic card with a description and an action function.
 * The CommunityChestCard class represents a community chest card, which is a type of Card.
 * The ChanceCard class represents a chance card, which is another type of Card.
 * 
 */

#pragma once

#ifndef CARDS_H
#define CARDS_H

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <memory>

class BoardManager; // Forward declaration

/**
 * @brief The Card class represents a generic card with a description and an action function.
 * 
 */
class Card
{
    private:
        std::string description; /** The description of the card. */
    protected:
        std::function<void(std::shared_ptr<BoardManager> )> _actionFunction; /** The action function associated with the card. */
    public:
        /**
         * @brief Constructs a new Card object with the given description.
         * 
         * @param _description The description of the card.
         */
        Card(std::string _description);

        /**
         * @brief Destroys the Card object.
         * 
         */
        ~Card();

        /**
         * @brief Gets the description of the card.
         * 
         * @return std::string The description of the card.
         */
        std::string getDescription() const;

        /**
         * @brief Performs the action associated with the card.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        virtual void action(std::shared_ptr<BoardManager> board) = 0;
};

/**
 * @brief The CommunityChestCard class represents a community chest card, which is a type of Card.
 * 
 */
class CommunityChestCard : public Card
{
    public:
        /**
         * @brief Constructs a new CommunityChestCard object with the given description and action function.
         * 
         * @param _description The description of the card.
         * @param _actionFunction The action function associated with the card.
         */
        CommunityChestCard(std::string _description, std::function<void(std::shared_ptr<BoardManager> )> _actionFunction);

        /**
         * @brief Destroys the CommunityChestCard object.
         * 
         */
        ~CommunityChestCard();

        /**
         * @brief Performs the action associated with the card.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Overloads the << operator to output the card's description.
         * 
         * @param os The output stream.
         * @param card The CommunityChestCard object.
         * @return std::ostream& The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const CommunityChestCard& card);
};

/**
 * @brief The ChanceCard class represents a chance card, which is another type of Card.
 * 
 */
class ChanceCard : public Card
{
    public:
        /**
         * @brief Constructs a new ChanceCard object with the given description and action function.
         * 
         * @param _description The description of the card.
         * @param _actionFunction The action function associated with the card.
         */
        ChanceCard(std::string _description, std::function<void(std::shared_ptr<BoardManager> )> _actionFunction);

        /**
         * @brief Destroys the ChanceCard object.
         * 
         */
        ~ChanceCard();

        /**
         * @brief Performs the action associated with the card.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Overloads the << operator to output the card's description.
         * 
         * @param os The output stream.
         * @param card The ChanceCard object.
         * @return std::ostream& The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const ChanceCard& card);
};

#endif // CARDS_H