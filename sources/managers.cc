/**
 * @file managers.cc
 * @brief Implementation file for the PlayerManager and BoardManager classes.
 *        These classes handle the management of players, buyable spaces, and the game board.
 * 
 * This file contains the implementation of the PlayerManager and BoardManager classes.
 * The PlayerManager class is responsible for managing players, including adding and removing players,
 * getting the current player, setting the next player, and transferring money between players.
 * 
 * The BoardManager class is responsible for managing the game board, including the spaces on the board,
 * the chance and community chest cards, and the player manager. It provides methods for drawing chance and
 * community chest cards, moving players on the board, and handling space actions.
 * 
 */

#include "managers.h"
#include <memory>
#include "../utils/iomanager.h"

Dice::Dice() : generator(std::random_device()()), distribution(1, 6) 
{
}


Dice::~Dice()
{
}

int Dice::roll()
{
    return distribution(generator);
}


PlayerManager::PlayerManager()
{
    std::ostringstream addressStream;
    addressStream << "PlayerManager created at address: " << static_cast<const void*>(this);
    logObject<std::string>(addressStream.str(), spdlog::level::debug);
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::addPlayer(std::shared_ptr<Player> player)
{
    players.push_back(player);
    if (currentPlayer == nullptr)
    {
        currentPlayer = player;
    }
}

void PlayerManager::removePlayer(std::shared_ptr<Player> player)
{
    // Look for the player in the list and remove it
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        if (*it == player)
        {
            players.erase(it);
            break;
        }
    }
}

std::shared_ptr<Player> PlayerManager::getPlayer(int index)
{
    return players[index];
}

int PlayerManager::getNbPlayers()
{
    return players.size();
}

std::shared_ptr<Player> PlayerManager::getCurrentPlayer()
{
    if (currentPlayer == nullptr) {
        spdlog::error("Current player is null!");
        return nullptr;
    }
    return currentPlayer;
}


void PlayerManager::setNextPlayer()
{
    // Find the index of the current player in the list and set the next player
    int currentPlayerIndex = std::distance(players.begin(), std::find(players.begin(), players.end(), currentPlayer));
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    currentPlayer = players[currentPlayerIndex];
}

void PlayerManager::setCurrentPlayer(std::shared_ptr<Player> player)
{
    currentPlayer = player;
}

void PlayerManager::transferMoneyFromTo(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, int amount)
{
    if (player1 != nullptr)
    {
        player1->setMoney(player1->getMoney() - amount);
        spdlog::debug("{0}'s money was {1}€ and is now {2}€.", player1->getName(), player1->getMoney() + amount, player1->getMoney());
        spdlog::info("{0} has been debited of {1}€.", player1->getName(), amount);
        if (player1->getMoney() < 0)
        {
            player1->setMoney(0);
            spdlog::debug("{0} has gone bankrupt while trying to transfer money. The transfer has however been completed by adding the missing money from the bank.", player1->getName());
        }
    }
    if (player2 != nullptr)
    {
        player2->setMoney(player2->getMoney() + amount); // In case of a bankrupcy from player1, we assume that the bank can add the missing money to player2 so that it still has the right amount of money
        spdlog::info("{0} has been credited of {1}€.", player2->getName(), amount);
    }
}

BoardManager::~BoardManager()
{
}

BoardManager::BoardManager(std::vector<std::shared_ptr<Space>> _board, std::vector<std::unique_ptr<CommunityChestCard>> _communityChestDeck, std::vector<std::unique_ptr<ChanceCard>> _chanceDeck)
    : board(_board), chanceDeck(std::move(_chanceDeck)), communityChestDeck(std::move(_communityChestDeck)), playerManager(std::make_shared<PlayerManager>())
{
    // Initialize the properties map with the properties of the board
    for (auto space : board) {
        if (auto property = std::dynamic_pointer_cast<Property>(space)) {
            Color color = property->getColor();
            properties[color].push_back(property);
        }
    }
    std::ostringstream addressStream;
    addressStream << "BoardManager created at address: " << static_cast<const void*>(this);
    logObject<std::string>(addressStream.str(), spdlog::level::debug);
}

std::shared_ptr<PlayerManager> BoardManager::getPlayerManager()
{
    return playerManager;
}

void BoardManager::drawChanceCard()
{
    if (chanceDeck.empty())
    {
        spdlog::error("Chance deck is empty.");
        return;
    }
    std::shuffle(chanceDeck.begin(), chanceDeck.end(), std::default_random_engine(std::random_device()())); // Shuffle the deck
    std::unique_ptr<ChanceCard>& card = chanceDeck[0]; // Draw the first card (only a reference to prevent moving the unique_ptr and losing the card)

    // If the card is a Get Out of Jail Free card and it has already been taken we draw another card
    if ((card->getDescription().find("Get Out of Jail Free") != std::string::npos) && chanceGOJFCTaken) 
    {
        drawChanceCard();
        return;
    }
    card->action(shared_from_this());
}

void BoardManager::setChanceGOJFCTaken(bool taken)
{
    chanceGOJFCTaken = taken;
}


void BoardManager::drawCommunityChestCard() 
{
    if (communityChestDeck.empty())
    {
        spdlog::error("Community Chest deck is empty.");
        return;
    }
    std::shuffle(communityChestDeck.begin(), communityChestDeck.end(), std::default_random_engine(std::random_device()())); // Shuffle the deck
    std::unique_ptr<CommunityChestCard>& card = communityChestDeck[0]; // Draw the first card (only a reference to prevent moving the unique_ptr and losing the card)

    // If the card is a Get Out of Jail Free card and it has already been taken we draw another card
    if ((card->getDescription().find("Get Out of Jail Free") != std::string::npos) && chanceGOJFCTaken)
    {
        drawCommunityChestCard();
        return;
    }
    card->action(shared_from_this());
}

void BoardManager::setCommunityChestGOJFCTaken(bool taken)
{
    communityChestGOJFCTaken = taken;
}

std::vector<std::shared_ptr<Space>> BoardManager::getBoard()
{
    return board;
}

std::vector<std::shared_ptr<Property>> BoardManager::getOwnedGroups(std::shared_ptr<Player>player){
    std::vector<std::shared_ptr<Property>> owned_properties;
    // Create a vector of properties owned by the player (only if the player owns all the properties of a group)
    for (const auto& [color, propertyList] : properties) {
        std::shared_ptr<Player> owner = player;
        bool sameOwner = true;

        for (const auto& property : propertyList) {
            if (property->getOwner() != owner) {
                sameOwner = false;
                break;
            }
        }

        if (sameOwner) {
            for(auto property : propertyList) {
                // Upgrade the property to full group if it is not already
                if (property->getNbBuildings() == PropertyRent::BASE_RENT){
                    property->setNbBuildings(PropertyRent::FULL_GROUP);
                }
                owned_properties.push_back(property);
            }
        }
    }
    return owned_properties;
}

void BoardManager::rollDice()
{
    currentDicesValue.first = dice1.roll();
    currentDicesValue.second = dice2.roll();
}

void BoardManager::movePlayer(int distanceToGo)
{
    std::shared_ptr<Player> player = playerManager->getCurrentPlayer();
    spdlog::debug("{0} is currently on {1}.", player->getName(), board[player->getPosition()]->getName());
    if (player == nullptr){
        spdlog::error("Player is null.");
        return;
    }
    int newPosition = (player->getPosition() + distanceToGo) % board.size();
    spdlog::debug("{0} moved to {1}.", player->getName(), board[newPosition]->getName());
    // Handle out-of-bounds position
    if ((newPosition < 0) || (newPosition >= static_cast<int>(board.size()))) {
        spdlog::error("New position is out of bounds.");
        return;
    }
    // Handle passing by the Go space
    if(newPosition < player->getPosition() && player->getRemainingTurnsInJail() == 0 && distanceToGo > 0){
        spdlog::info("{0} passed by the Go space and earned 200.", player->getName());
        playerManager->transferMoneyFromTo(nullptr, player, 200);
    }
    // Set the new position of the player
    spdlog::info("Moving player...");
    player->setPosition(newPosition);
    handleSpace();
}

void BoardManager::affectOwnership(std::shared_ptr<Player> player, std::shared_ptr<Space> space)
{
    // Check if the space is a buyable space and set the owner to the player
    std::shared_ptr<BuyableSpace> buyableSpace = std::dynamic_pointer_cast<BuyableSpace>(space);
    if (buyableSpace != nullptr) {
        buyableSpace->setOwner(player);
    }
}

void BoardManager::buildOnProperties(std::vector<std::shared_ptr<Property>> properties, std::shared_ptr<Player> player, int attempts)
{
    // Security to avoid infinite loop (When testing, the values are random so it is possible to have a difference of more than 1 building between properties multiple times in a row)
    if (attempts > 50) {
        spdlog::debug("Exiting to avoid infinite loop.");
        spdlog::error("Too many attempts. Exiting.");
        return;
    }

    std::map<std::shared_ptr<Property>, int> buildingsPerProperty;
    int minBuildings = INT_MAX;
    int maxBuildings = INT_MIN;
    int totalNewBuildings = 0;

    spdlog::info("With the money you currently have, you can build up to {0} buildings in total.", player->getMoney() / properties[0]->getHousePrice());
    // Ask the player how many buildings they want to build on each property
    for (const auto& property : properties) {
        int currentBuildingsOnProperty = static_cast<int>(property->getNbBuildings());
        int maxTheoretical = std::min(5 - currentBuildingsOnProperty, player->getMoney() / properties[0]->getHousePrice());
        int additionalBuildingsOnProperty = getNumber("Enter the number of additional buildings you want to build on " + property->getName() + ": ", 0, maxTheoretical);
        totalNewBuildings += additionalBuildingsOnProperty;
        int totalBuildingsOnProperty = currentBuildingsOnProperty + additionalBuildingsOnProperty;
        spdlog::debug("Theoretical number of buildings on this property: {0}.", totalBuildingsOnProperty);
        if (totalBuildingsOnProperty == 5) {
            spdlog::info("You have chosen to build a hotel on {0}.", property->getName());
        } else {
            spdlog::info("You have chosen to build {0} house(s) on {1}.", additionalBuildingsOnProperty, property->getName());
        }
        buildingsPerProperty[property] = totalBuildingsOnProperty;
        minBuildings = std::min(minBuildings, totalBuildingsOnProperty);
        maxBuildings = std::max(maxBuildings, totalBuildingsOnProperty);
    }

    // Check if the number of buildings is consistent between properties (maximal difference of 1 building allowed)
    if (maxBuildings - minBuildings > 1) {
        spdlog::error("There is a difference of more than 1 building between some properties.");
        return buildOnProperties(properties, player, attempts + 1);
    }

    if (totalNewBuildings * properties[0]->getHousePrice() > player->getMoney()) {
        spdlog::error("You don't have enough money to build all these properties.");
        return;
    }else {
        playerManager->transferMoneyFromTo(player, nullptr, totalNewBuildings * properties[0]->getHousePrice());
    }

    // Build the houses/hotel on the properties and transfer the money from the player to the bank
    for (const auto& [property, totalBuildings] : buildingsPerProperty) {
        property->setNbBuildings(static_cast<PropertyRent>(totalBuildings));
        spdlog::info("{0} now has {1} {2}.", property->getName(), totalBuildings == 5 ? 1 : totalBuildings, totalBuildings == 5 ? "hotel" : "house(s)");
    }
}

void BoardManager::handleSpace()
{
    // Execute the action of the space the player is on
    std::shared_ptr<Player> player = playerManager->getCurrentPlayer();
    std::shared_ptr<Space> space = board[player->getPosition()];
    spdlog::info("{0} is now on {1} ({2}).", player->getName(), space->getName(), player->getPosition());
    space->action(shared_from_this());
}

std::pair<int, int> BoardManager::getCurrentDicesValue()
{
    return currentDicesValue;
}