/**
 * @file gamecore.cc
 * @brief Implementation of the GameCore class
 * 
 * This file contains the implementation of the GameCore class, which handles the game core logic.
 * The GameCore class manages the players, spaces, cards, and the flow of the game.
 * 
 */

#include "gamecore.h"
#include <iostream>
#include <memory>
#include "../utils/iomanager.h"



GameCore::GameCore(std::vector<std::shared_ptr<Space>> spaces, std::vector<std::unique_ptr<CommunityChestCard>> communityChestCards, std::vector<std::unique_ptr<ChanceCard>> chanceCards)
    : boardManager(std::make_shared<BoardManager>(spaces, std::move(communityChestCards), std::move(chanceCards)))
{
    std::ostringstream addressStream;
    addressStream << "GameCore created at address: " << static_cast<const void*>(this);
    logObject<std::string>(addressStream.str(), spdlog::level::debug);
}

GameCore::~GameCore()
{
}


void GameCore::startGame()
{
    spdlog::info("MONOPOLY GAME");
    spdlog::info("Welcome to the Monopoly game!");
    int nbPlayers = getNumber("Enter the number of players (2-8): ", 2, 8);
    //create players
    for (int i = 0; i < nbPlayers; i++)
    {
        std::string message = "Enter the name of player " + std::to_string(i + 1) + ": ";
        std::string name = getString(message);
        boardManager->getPlayerManager()->addPlayer(std::make_shared<Player>(name));
    }
    getEnter("Press Enter to start the game.");
    int turns = 0;
    //game loop (exit when only one player left or after 1000 turns to avoid infinite loop)
    while (boardManager->getPlayerManager()->getNbPlayers() > 1 && turns < 1000){
        playTurn();
        turns++;
    }
    spdlog::info("Game over!");
    if (turns == 1000)
    {
        spdlog::info("The game has reached the maximum number of turns (1000).");
        return;
    }
    spdlog::info("The winner is {0}!", boardManager->getPlayerManager()->getPlayer(0)->getName());
}

void GameCore::playTurn()
{
    if (boardManager->getPlayerManager()->getCurrentPlayer() == nullptr)
    {
        boardManager->getPlayerManager()->setNextPlayer();
        return;
    }
    spdlog::info("It is {0}'s turn ({1}€).", boardManager->getPlayerManager()->getCurrentPlayer()->getName(), boardManager->getPlayerManager()->getCurrentPlayer()->getMoney());
    // If the player is in jail, execute the jail logic
    if (boardManager->getPlayerManager()->getCurrentPlayer()->getRemainingTurnsInJail() > 0){
        boardManager->handleSpace();
        boardManager->getPlayerManager()->setNextPlayer();
        return;
    }
    getEnter("Press Enter to roll the dice.");
    boardManager->rollDice();
    std::pair<int, int> dicesValue = boardManager->getCurrentDicesValue();
    spdlog::info("You rolled a {0} and a {1}.", dicesValue.first, dicesValue.second);
    boardManager->movePlayer(dicesValue.first + dicesValue.second);
    if (dicesValue.first == dicesValue.second)
    {
        // Track consecutive doubles to handle the "Go to jail" rule
        consecutiveDoubles++;
        if (consecutiveDoubles == 3)
        {
            spdlog::info("You rolled 3 doubles in a row! Go to jail.");
            boardManager->getPlayerManager()->getCurrentPlayer()->setPosition(10); // Move player to jail without passing Go
            boardManager->getPlayerManager()->getCurrentPlayer()->setRemainingTurnsInJail(3);
            boardManager->handleSpace();
            boardManager->getPlayerManager()->setNextPlayer();
            consecutiveDoubles = 0;
            return;
        }
        // Prevent the player from playing again if they went to jail but rolled a double before
        if (boardManager->getPlayerManager()->getCurrentPlayer()->getRemainingTurnsInJail() > 0){
            return;
        }else if (boardManager->getPlayerManager()->getCurrentPlayer()->getMoney() > 0){
            spdlog::info("You rolled a double! You get to play again.");
            playTurn();
        }
    }
    else
    {
        // If the player is bankrupt, don't offer them the option to buy properties
        if(boardManager->getPlayerManager()->getCurrentPlayer()->getMoney() > 0){        
            std::vector<std::shared_ptr<BuyableSpace>> owned_properties;
            for (const auto& space : boardManager->getBoard())
            {
                // Get the properties owned by the player
                std::shared_ptr<BuyableSpace> buyableSpace = std::dynamic_pointer_cast<BuyableSpace>(space);
                if (buyableSpace != nullptr && buyableSpace->getOwner() == boardManager->getPlayerManager()->getCurrentPlayer())
                {
                    owned_properties.push_back(buyableSpace);
                }
            }
            if (owned_properties.size() > 0)
            {
                spdlog::info("You own the following spaces:");
                for (int i = 0; i < owned_properties.size(); i++)
                {
                    spdlog::info("{0}. {1}", i + 1, owned_properties[i]->getName());
                }
            }
            while(true)
            {            
                // Get the properties that can be built on
                std::vector<std::shared_ptr<Property>> owned_groups = boardManager->getOwnedGroups(boardManager->getPlayerManager()->getCurrentPlayer());
                if (owned_groups.size() > 0){
                    std::map<std::string, std::vector<std::shared_ptr<Property>>> propertiesByColor;
                    for (const auto& property : owned_groups) {
                        propertiesByColor[property->getColorString()].push_back(property);
                        spdlog::debug("Property {0} added to color {1}", property->getName(), property->getColorString());
                    }
                    for (auto it = propertiesByColor.begin(); it != propertiesByColor.end(); ) {
                        bool allHaveHotel = true;
                        for (const auto& property : it->second) {
                            if (property->getNbBuildings() != PropertyRent::HOTEL) {
                                allHaveHotel = false;
                                break;
                            }
                        }
                        if (allHaveHotel) {
                            spdlog::debug("Removing color: {0}", it->first);
                            it = propertiesByColor.erase(it);
                        } else {
                            ++it;
                        }
                    }
                    // Display the properties that can be built on
                    if (propertiesByColor.size() > 0) {
                        std::vector<std::string> colors;
                        spdlog::info("You can build on the following properties:");
                        for (const auto& [color, properties] : propertiesByColor) {
                            spdlog::info("Color: {0}", color);
                            colors.push_back(color);
                            for (int i = 0; i < properties.size(); i++) {
                                spdlog::info("  {0}. {1}", i + 1, properties[i]->getName());
                                PropertyRent buildings = properties[i]->getNbBuildings();
                                switch (buildings) {
                                    case PropertyRent::FULL_GROUP:
                                        spdlog::info("     - Buildings: None (Price of a house: {0}€)", properties[i]->getHousePrice());
                                        break;
                                    case PropertyRent::ONE_HOUSE:
                                    case PropertyRent::TWO_HOUSES:
                                    case PropertyRent::THREE_HOUSES:
                                        spdlog::info("     - Buildings: {0} house(s) (Price of another house: {1}€/house)", static_cast<int>(buildings), properties[i]->getHousePrice());
                                        break;
                                    case PropertyRent::FOUR_HOUSES:
                                        spdlog::info("     - Buildings: {0} house(s) (Price of a hotel: {1}€)", static_cast<int>(buildings), properties[i]->getHousePrice());
                                        break;
                                    case PropertyRent::HOTEL:
                                        spdlog::info("     - Buildings: Hotel");
                                        break;
                                    case PropertyRent::BASE_RENT:
                                        spdlog::error("    This should not happen, .");
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                        // Ask the player if they want to build on a property
                        char build = getYesNo("Do you want to build on a property? (y/n)");
                        if (build == 'y')
                        {   
                            std::string color = getString("Enter the color of the property you want to build on: ", colors);
                            if(propertiesByColor.count(color) > 0){
                                boardManager->buildOnProperties(propertiesByColor[color], boardManager->getPlayerManager()->getCurrentPlayer());
                            }else{
                                spdlog::error("{0} is an invalid color.", color);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    // Check for a player bankruptcy
    for (int i = 0; i < boardManager->getPlayerManager()->getNbPlayers(); i++)
    {
        std::shared_ptr<Player> player = boardManager->getPlayerManager()->getPlayer(i);
        if (player->getMoney() == 0)
        {
            spdlog::info("{0} is bankrupt!", player->getName());
            // Make all the properties owned by the player available for purchase and remove the player from the game
            for (const auto& space : boardManager->getBoard())
            {
                std::shared_ptr<BuyableSpace> buyableSpace = std::dynamic_pointer_cast<BuyableSpace>(space);
                if (buyableSpace != nullptr && buyableSpace->getOwner() == player)
                {
                    buyableSpace->setOwner(nullptr);
                    std::shared_ptr<Property> property = std::dynamic_pointer_cast<Property>(buyableSpace);
                    if (property != nullptr)
                    {
                        property->setNbBuildings(PropertyRent::BASE_RENT);
                    }
                    spdlog::info("{0} is now available for purchase.", buyableSpace->getName());
                }
            }
            boardManager->getPlayerManager()->removePlayer(player);
        }
    }
    // Next player
    boardManager->getPlayerManager()->setNextPlayer();
    consecutiveDoubles = 0;
}





