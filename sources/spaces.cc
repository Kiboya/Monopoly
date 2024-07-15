/**
 * @file spaces.cc
 * @brief Implementation of the Space, BuyableSpace, Property, Station, Utility, Tax, Jail, GoToJail, FreeParking, Go, CommunityChest, and Chance classes
 * 
 * This file contains the implementation of various classes related to the spaces in a Monopoly game.
 * The classes include Space, BuyableSpace, Property, Station, Utility, Tax, Jail, GoToJail, FreeParking, Go, CommunityChest, and Chance.
 * These classes define the behavior and properties of different types of spaces on the game board.
 */

#include "spaces.h"
#include <iostream>
#include <sstream>
#include "../utils/iomanager.h"


Space::Space(std::string _name) : name(_name)
{
}

Space::~Space()
{
}

std::string Space::getName() const
{
    return name;
}

BuyableSpace::BuyableSpace(std::string _name, int _price, std::vector<int> _rent) : Space(_name), price(_price), rent(_rent), owner(nullptr)
{
}

BuyableSpace::~BuyableSpace()
{
}

int BuyableSpace::getPrice() const
{
    return price;
}

std::shared_ptr<Player> BuyableSpace::getOwner() const
{
    return owner;
}

void BuyableSpace::setOwner(std::shared_ptr<Player> _owner)
{
    owner = _owner;
}

std::vector<int> BuyableSpace::getRent() const
{
    return rent;
}

std::string colorToString(Color color){
    switch (color)
    {
    case Color::PURPLE:
        return "purple";
        break;
    case Color::LIGHT_BLUE:
        return "light blue";
        break;
    case Color::PINK:
        return "pink";
        break;
    case Color::ORANGE:
        return "orange";
        break;
    case Color::RED:
        return "red";
        break;
    case Color::YELLOW:
        return "yellow";
        break;
    case Color::GREEN:
        return "green";
        break;
    case Color::BLUE:
        return "blue";
        break;
    default:
        return "unknown";
        break;
    }
}

Property::Property(std::string _name, Color _color, int _price, int _housePrice, std::vector<int> _rent) : BuyableSpace(_name, _price, _rent), housePrice(_housePrice), color(_color), nbBuildings(PropertyRent::BASE_RENT)
{
}

Property::~Property()
{
}

int Property::getHousePrice() const
{
    return housePrice;
}

void Property::action(std::shared_ptr<BoardManager> board)
{
    std::shared_ptr<Player> player = board->getPlayerManager()->getCurrentPlayer();
    if (getOwner() != nullptr)
    {
        if (getOwner() == player){
            spdlog::info("You own this property");
            return;
        }
        // Make the player pay rent
        spdlog::info("This property is owned by {0}", getOwner()->getName());
        int rent = getRent()[static_cast<int>(nbBuildings)];
        spdlog::info("{0} needs to pay {1}€ to {2}", player->getName(), rent, getOwner()->getName());
        board->getPlayerManager()->transferMoneyFromTo(player, getOwner(), rent);

    }
    else
    {
        // Offer the player to buy the property or auction it
        spdlog::info("This property is not owned");
        logObject(*this);     
        std::string message = "Do you want to buy it for " + std::to_string(getPrice()) + "€ ? [y/n]";
        char answer = getYesNo(message);
        if (answer == 'y')
        {
            if (player->getMoney() < getPrice())
            {
                spdlog::info("You don't have enough money to buy this property");
                auction(board);
            }
            else
            {
                board->getPlayerManager()->transferMoneyFromTo(player, nullptr, getPrice());
                board->affectOwnership(player, shared_from_this());
                spdlog::info("You now own this property");
            }
        }
        else
        {
            auction(board);
        }
    }
}

void Property::auction(std::shared_ptr<BoardManager> board)
{
    spdlog::info("You decided not to buy this property and it will now be auctioned");
    // Get all players in the game
    std::vector<std::shared_ptr<Player>> auctionPlayers(board->getPlayerManager()->getNbPlayers());
    for (int i = 0; i < board->getPlayerManager()->getNbPlayers(); i++)
    {
        auctionPlayers[i] = board->getPlayerManager()->getPlayer(i);
    }    
    int bid = 0;
    // Start the auction
    while (auctionPlayers.size() > 1)
    {
        // Make the first player bid (remove him from the list and add him back at the end if he bids)
        std::shared_ptr<Player> currentPlayer = auctionPlayers.front();
        auctionPlayers.erase(auctionPlayers.begin());
        spdlog::info("It is {0}'s turn to bid.", currentPlayer->getName());
        if (currentPlayer->getMoney() - 1 <= bid)
        {
            spdlog::info("{0} doesn't have enough money to bid and is out of the auction", currentPlayer->getName());
        }
        else
        {
            std::string message = "Do you want to bid on this property? [y/n]";
            char answer = getYesNo(message);
            if (answer == 'y')
            {
                bid = getNumber("How much do you want to bid?", bid+1, currentPlayer->getMoney() - 1);
                spdlog::info("{0} is now the highest bidder with a bid of {1}€", currentPlayer->getName(), bid);
                auctionPlayers.push_back(currentPlayer);
            }
            else
            {
                spdlog::info("{0} decided not to bid and is out of the auction", currentPlayer->getName());
            }
        }
    }
    std::shared_ptr<Player> winner = auctionPlayers.front();
    spdlog::info("{0} won the auction with a bid of {1}€", winner->getName(), bid);
    board->getPlayerManager()->transferMoneyFromTo(winner, nullptr, bid);
    board->affectOwnership(winner, shared_from_this());
    spdlog::info("{0} now owns this property", winner->getName());
}

PropertyRent Property::getNbBuildings() const
{
    return nbBuildings;
}

void Property::setNbBuildings(PropertyRent _nbBuildings)
{
    nbBuildings = _nbBuildings;
}

Color Property::getColor() const
{
    return color;
}

std::string Property::getColorString() const
{
    return colorToString(color);
}

std::ostream& operator<<(std::ostream& os, const Property& property)
{
    os << "Space Type: Property" << std::endl;
    os << "Property Name: " << property.getName() << std::endl;
    os << "Property Color: " << colorToString(property.getColor()) << std::endl;
    os << "Property Price: " << property.getPrice() << "€" << std::endl;
    os << "Property Rent: " << std::endl;
    for (int i = 0; i < property.getRent().size(); i++)
    {
        os << "     With ";
        switch(i){
            case 0:
                os << "full group: ";
                break;
            case 1: case 2: case 3: case 4:
                os << i << " house(s): ";
                break;
            case 5:
                os << "a hotel: ";
                break;
            case 6:
                os << "base rent: ";
                break;
        }
        os << property.getRent()[i] << "€" << std::endl;
    }
    os << "Property House Price: " << property.housePrice << "€" << std::endl;
    return os;
}

Station::Station(std::string _name) : BuyableSpace(_name, 200, {25, 50, 100, 200})
{
}

Station::~Station()
{
}

void Station::action(std::shared_ptr<BoardManager> board)
{
    std::shared_ptr<Player> player = board->getPlayerManager()->getCurrentPlayer();
    if (getOwner() != nullptr)
    {
        if (getOwner() == player){
            spdlog::info("You own this station");
            return;
        }
        // Pay rent
        spdlog::info("This station is owned by {0}", getOwner()->getName());
        int rent = getRent()[getOwner()->getNbStationsOwned() - 1];
        spdlog::info("{0} owns {1} station(s)", getOwner()->getName(), getOwner()->getNbStationsOwned());
        spdlog::info("{0} needs to pay {1}€ to {2}", player->getName(), rent, getOwner()->getName());
        board->getPlayerManager()->transferMoneyFromTo(player, getOwner(), rent);
    }
    else
    {
        // Offer the player to buy the station or auction it
        spdlog::info("This station is not owned");
        logObject(*this);
        std::string message = "Do you want to buy it for " + std::to_string(getPrice()) + "€ ? [y/n]";
        char answer = getYesNo(message);
        if (answer == 'y')
        {
            if (player->getMoney() < getPrice())
            {
                spdlog::info("You don't have enough money to buy this station");
                auction(board);
            }
            else
            {
                board->getPlayerManager()->transferMoneyFromTo(player, nullptr, getPrice());
                board->affectOwnership(player, shared_from_this());
                spdlog::info("You now own this this station");
                player->setNbStationsOwned(player->getNbStationsOwned() + 1);
            }
        }
        else
        {
            auction(board);
        }
    }
}

void Station::auction(std::shared_ptr<BoardManager> board)
{
    spdlog::info("You decided not to buy this station and it will now be auctioned");
    // Get all players in the game
    std::vector<std::shared_ptr<Player>> auctionPlayers(board->getPlayerManager()->getNbPlayers());
    for (int i = 0; i < board->getPlayerManager()->getNbPlayers(); i++)
    {
        auctionPlayers[i] = board->getPlayerManager()->getPlayer(i);
    }
    int bid = 0;
    // Start the auction
    while (auctionPlayers.size() > 1)
    {
        // Make the first player bid (remove him from the list and add him back at the end if he bids)
        std::shared_ptr<Player> currentPlayer = auctionPlayers.front();
        auctionPlayers.erase(auctionPlayers.begin());
        spdlog::info("It is {0}'s turn to bid.", currentPlayer->getName());
        if (currentPlayer->getMoney() - 1 <= bid)
        {
            spdlog::info("{0} doesn't have enough money to bid and is out of the auction", currentPlayer->getName());
        }
        else
        {
            std::string message = "Do you want to bid on this station? [y/n]";
            char answer = getYesNo(message);
            if (answer == 'y')
            {
                bid = getNumber("How much do you want to bid?", bid+1, currentPlayer->getMoney() - 1);
                spdlog::info("{0} is now the highest bidder with a bid of {1}€", currentPlayer->getName(), bid);
                auctionPlayers.push_back(currentPlayer);
            }
            else
            {
                spdlog::info("{0} decided not to bid and is out of the auction", currentPlayer->getName());
            }
        }
    }
    std::shared_ptr<Player> winner = auctionPlayers.front();
    spdlog::info("{0} won the auction with a bid of {1}€", winner->getName(), bid);
    board->getPlayerManager()->transferMoneyFromTo(winner, nullptr, bid);
    board->affectOwnership(winner, shared_from_this());
    winner->setNbStationsOwned(winner->getNbStationsOwned() + 1);
    spdlog::info("{0} now owns this station", winner->getName());
}

std::ostream& operator<<(std::ostream& os, const Station& station)
{
    os << "Space Type: Station" << std::endl;
    os << "Station Name: " << station.getName() << std::endl;
    os << "Station Price: " << station.getPrice() << "€" << std::endl;
    os << "Station Rent: " << std::endl;
    for (int i = 0; i < station.getRent().size(); i++)
    {
        os << "     With " << i << " station(s): ";
        if (i < station.getRent().size())
        {
            os << station.getRent()[i] << "€";
        }
        else
        {
            os << "N/A";
        }
        os << std::endl;
    }
    return os;
}

Utility::Utility(std::string _name) : BuyableSpace(_name, 150, {})
{
}

Utility::~Utility()
{
}

void Utility::action(std::shared_ptr<BoardManager> board)
{
    std::shared_ptr<Player> player = board->getPlayerManager()->getCurrentPlayer();
    if (getOwner() != nullptr)
    {
        if (getOwner() == player){
            spdlog::info("You own this utility");
            return;
        }
        // Pay rent
        spdlog::info("This utility is owned by {0}", getOwner()->getName());
        std::pair<int, int> dicesValue = board->getCurrentDicesValue();
        spdlog::info("The rent is ({0} + {1}) * {2}€", dicesValue.first, dicesValue.second, (getOwner()->getNbUtilitiesOwned() == 1 ? 4 : 10));
        int rent = (dicesValue.first + dicesValue.second) * (getOwner()->getNbUtilitiesOwned() == 1 ? 4 : 10);
        spdlog::info("{0} needs to pay {1}€ to {2}", player->getName(), rent, getOwner()->getName());
        board->getPlayerManager()->transferMoneyFromTo(player, getOwner(), rent);
    }
    else
    {
        // Offer the player to buy the utility or auction it
        spdlog::info("This utility is not owned");
        logObject(*this);
        std::string message = "Do you want to buy it for " + std::to_string(getPrice()) + "€ ? [y/n]";
        char answer = getYesNo(message);
        if (answer == 'y')
        {
            if (player->getMoney() < getPrice())
            {
                spdlog::info("You don't have enough money to buy this utility");
                auction(board);
            }
            else
            {
                board->getPlayerManager()->transferMoneyFromTo(player, nullptr, getPrice());
                board->affectOwnership(player, shared_from_this());
                spdlog::info("You now own this utility");
                player->setNbUtilitiesOwned(player->getNbUtilitiesOwned() + 1);
            }
        }
        else
        {
            auction(board);
        }
    }
}

void Utility::auction(std::shared_ptr<BoardManager> board)
{
    spdlog::info("You decided not to buy this utility and it will now be auctioned");
    // Get all players in the game
    std::vector<std::shared_ptr<Player>> auctionPlayers(board->getPlayerManager()->getNbPlayers());
    for (int i = 0; i < board->getPlayerManager()->getNbPlayers(); i++)
    {
        auctionPlayers[i] = board->getPlayerManager()->getPlayer(i);
    }    
    int bid = 0;
    // Start the auction
    while (auctionPlayers.size() > 1)
    {
        // Make the first player bid (remove him from the list and add him back at the end if he bids)
        std::shared_ptr<Player> currentPlayer = auctionPlayers.front();
        auctionPlayers.erase(auctionPlayers.begin());
        spdlog::info("It is {0}'s turn to bid.", currentPlayer->getName());
        if (currentPlayer->getMoney() - 1 <= bid)
        {
            spdlog::info("{0} doesn't have enough money to bid and is out of the auction", currentPlayer->getName());
        }
        else
        {
            std::string message = "Do you want to bid on this utility? [y/n]";
            char answer = getYesNo(message);
            if (answer == 'y')
            {
                bid = getNumber("How much do you want to bid?", bid+1, currentPlayer->getMoney() - 1);
                spdlog::info("{0} is now the highest bidder with a bid of {1}€", currentPlayer->getName(), bid);
                auctionPlayers.push_back(currentPlayer);
            }
            else
            {
                spdlog::info("{0} decided not to bid and is out of the auction", currentPlayer->getName());
            }
        }
    }
    std::shared_ptr<Player> winner = auctionPlayers.front();
    spdlog::info("{0} won the auction with a bid of {1}€", winner->getName(), bid);
    board->getPlayerManager()->transferMoneyFromTo(winner, nullptr, bid);
    board->affectOwnership(winner, shared_from_this());
    winner->setNbUtilitiesOwned(winner->getNbUtilitiesOwned() + 1);
    spdlog::info("{0} now owns this utility", winner->getName());
}

std::ostream& operator<<(std::ostream& os, const Utility& utility)
{
    os << "Space Type: Utility" << std::endl;
    os << "Utility Name: " << utility.getName() << std::endl;
    os << "Utility Price: " << utility.getPrice() << std::endl;
    os << "Utility Rent: "<< std::endl; 
    os << "     4x the dice roll if one utility is owned, or 10x the dice value if both are owned" << std::endl;
    return os;
}

Tax::Tax(std::string _name, int _amount) : Space(_name), amount(_amount)
{
}

Tax::~Tax()
{
}

int Tax::getAmount() const
{
    return amount;
}

void Tax::action(std::shared_ptr<BoardManager> board)
{
    std::shared_ptr<Player> player = board->getPlayerManager()->getCurrentPlayer();
    spdlog::info("{0} needs to pay {1}€ to the bank", player->getName(), amount);
    board->getPlayerManager()->transferMoneyFromTo(player, nullptr, amount);

}

std::ostream& operator<<(std::ostream& os, const Tax& tax)
{
    os << "Space Type: Tax" << std::endl;
    os << "Tax Name: " << tax.getName() << std::endl;
    os << "Tax Amount: " << tax.getAmount() << "€" << std::endl;
    return os;
}

Jail::Jail() : Space("Jail")
{
}

Jail::~Jail()
{
}

void Jail::action(std::shared_ptr<BoardManager> board)
{
    std::shared_ptr<Player> player = board->getPlayerManager()->getCurrentPlayer();
    if (player->getRemainingTurnsInJail() > 0)
    {
        // If the player has a Get Out of Jail Free card, he can use it
        spdlog::info("You are stuck in jail for {0} turn(s)", player->getRemainingTurnsInJail());
        if (player->getHasChanceGOJFC() || player->getHasCommunityChestGOJFC())
        {
            std::string message = "Do you want to use your Get Out of Jail Free card? [y/n]";
            char answer = getYesNo(message);
            if (answer == 'y')
            {
                player->setRemainingTurnsInJail(0);
                if (player->getHasChanceGOJFC())
                {
                    player->setHasChanceGOJFC(false);
                    board->setChanceGOJFCTaken(false);
                }
                else
                {
                    player->setHasCommunityChestGOJFC(false);
                    board->setCommunityChestGOJFCTaken(false);
                }
                spdlog::info("You used your card and are free to go");
                return;
            }
        }
        // Offer the player to pay 50 to get out of jail or roll the dice
        std::string message = "Do you want to pay 50 to get out of jail? [y/n]";
        char answer = getYesNo(message);
        if (answer == 'y')
        {
            if (player->getMoney() < 50)
            {
                spdlog::info("You don't have enough money to pay the fine");
            }
            else
            {
                board->getPlayerManager()->transferMoneyFromTo(player, nullptr, 50);
                player->setRemainingTurnsInJail(0);
                spdlog::info("You are free to go");
                return;
            }
        }
        else
        {
            spdlog::info("Roll the dice and get a double to get out of jail");
            board->rollDice();
            std::pair<int, int> dicesValue = board->getCurrentDicesValue();
            spdlog::info("You rolled a {0} and a {1}.", dicesValue.first, dicesValue.second);
            if (dicesValue.first == dicesValue.second)
            {
                player->setRemainingTurnsInJail(0);
                spdlog::info("You rolled a double are free to go!");
                return;
            }else{
                spdlog::info("You didn't get a double");
            }
        }
        player->setRemainingTurnsInJail(player->getRemainingTurnsInJail() - 1);
        spdlog::info("You have {0} turn(s) left in jail", player->getRemainingTurnsInJail());
    }
    else
    {
        spdlog::info("You are free to go!");
    }
} 

std::ostream& operator<<(std::ostream& os, const Jail& jail)
{
    os << "Space Type: Jail" << std::endl;
    return os;
}

GoToJail::GoToJail() : Space("Go To Jail")
{
}

GoToJail::~GoToJail()
{
}

void GoToJail::action(std::shared_ptr<BoardManager> board)
{
    std::shared_ptr<Player> player = board->getPlayerManager()->getCurrentPlayer();
    spdlog::info("You are going to jail");
    player->setRemainingTurnsInJail(3);
    board->movePlayer(20);
}

std::ostream& operator<<(std::ostream& os, const GoToJail& goToJail)
{
    os << "Space Type: Go To Jail" << std::endl;
    return os;
}

FreeParking::FreeParking() : Space("Free Parking")
{
}

FreeParking::~FreeParking()
{
}

void FreeParking::action(std::shared_ptr<BoardManager> board)
{
    spdlog::info("You landed on Free Parking");
    spdlog::info("There is nothing to do here, enjoy your stay");
}

std::ostream& operator<<(std::ostream& os, const FreeParking& freeParking)
{
    os << "Space Type: Free Parking" << std::endl;
    return os;
}

Go::Go() : Space("Go")
{
}

Go::~Go()
{
}

void Go::action(std::shared_ptr<BoardManager> board)
{
    // Give an extra 200 to the player who lands exactly on the Go space
    std::shared_ptr<Player> player = board->getPlayerManager()->getCurrentPlayer();
    spdlog::info("{0} landed exactly on the Go space and earned an extra 200", player->getName());
    board->getPlayerManager()->transferMoneyFromTo(nullptr, player, 200);
}

std::ostream& operator<<(std::ostream& os, const Go& go)
{
    os << "Space Type: Go" << std::endl;
    return os;
}

CommunityChest::CommunityChest() : Space("Community Chest")
{
}

CommunityChest::~CommunityChest()
{
}

void CommunityChest::action(std::shared_ptr<BoardManager> board)
{
    board->drawCommunityChestCard();
}

std::ostream& operator<<(std::ostream& os, const CommunityChest& communityChest)
{
    os << "Space Type: Community Chest" << std::endl;
    return os;
}

Chance::Chance() : Space("Chance")
{
}

Chance::~Chance()
{
}

void Chance::action(std::shared_ptr<BoardManager> board)
{
    board->drawChanceCard();
}


std::ostream& operator<<(std::ostream& os, const Chance& chance)
{
    os << "Space Type: Chance" << std::endl;
    return os;
}
