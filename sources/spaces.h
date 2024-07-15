/**
 * @file spaces.h
 * @brief This file contains the declaration of classes related to spaces on the game board.
 *        It includes the base class Space, derived classes BuyableSpace, Property, Station, and Utility,
 *        as well as the enumerations Color and PropertyRent.
 *        It also includes classes for specific spaces like Tax, Jail, GoToJail, FreeParking, Go, CommunityChest, and Chance.
 * 
 */
#pragma once

#ifndef SPACES_H
#define SPACES_H

#include "managers.h"
#include "player.h"
#include <memory>
#include <vector>

class BoardManager; // Forward declaration

/**
 * @class Space
 * @brief Represents a space on the board.
 * 
 * This class serves as a base class for different types of spaces on the board.
 * It provides common functionality and defines an interface for derived classes.
 */
class Space : public std::enable_shared_from_this<Space>
{
    private:
        std::string name; /**< The name of the space. */
    public:
        /**
         * @brief Constructs a Space object with the given name.
         * @param _name The name of the space.
         */
        Space(std::string _name);

        /**
         * @brief Destroys the Space object.
         */
        ~Space();

        /**
         * @brief Gets the name of the space.
         * @return The name of the space.
         */
        std::string getName() const;

        /**
         * @brief Performs the action associated with the space.
         * @param board A shared pointer to the BoardManager object.
         * 
         * This is a pure virtual function that must be implemented by derived classes.
         * It defines the action that should be performed when a player lands on this space.
         */
        virtual void action(std::shared_ptr<BoardManager> board)  = 0;
};

/**
 * @class BuyableSpace
 * @brief Represents a buyable space on a game board.
 * 
 * This class is a derived class of the Space class and provides functionality for buyable spaces.
 * A buyable space can be purchased by a player and has a price, rent, and owner.
 */
class BuyableSpace : public Space
{
    private:/**
 * @brief 
 * 
 */   int price; /**< The price of the buyable space. */
        std::vector<int> rent; /**< The rent of the buyable space. */
        std::shared_ptr<Player> owner; /**< The owner of the buyable space. */
    public:
        /**
         * @brief Constructs a BuyableSpace object with the specified name, price, and rent.
         * @param _name The name of the buyable space.
         * @param _price The price of the buyable space.
         * @param _rent The rent of the buyable space.
         */
        BuyableSpace(std::string _name, int _price, std::vector<int> _rent);

        /**
         * @brief Destroys the BuyableSpace object.
         */
        ~BuyableSpace();

        /**
         * @brief Gets the price of the buyable space.
         * @return The price of the buyable space.
         */
        int getPrice() const;

        /**
         * @brief Gets the owner of the buyable space.
         * @return The owner of the buyable space.
         */
        std::shared_ptr<Player> getOwner() const;

        /**
         * @brief Sets the owner of the buyable space.
         * @param _owner The owner of the buyable space.
         */
        void setOwner(std::shared_ptr<Player> _owner);

        /**
         * @brief Gets the rent of the buyable space.
         * @return The rent of the buyable space.
         */
        std::vector<int> getRent() const;

        /**
         * @brief Performs the action associated with the buyable space.
         * @param board The game board manager.
         */
        virtual void action(std::shared_ptr<BoardManager> board)  = 0;

        /**
         * @brief Performs an auction for the buyable space.
         * @param board The game board manager.
         */
        virtual void auction(std::shared_ptr<BoardManager> board) = 0;
};

/**
 * @enum Color
 * @brief Represents the color of a property.
 * 
 * The Color enum class defines the different colors that a property can have.
 * These colors are used to categorize properties on the game board.
 */
enum class Color
{
    PURPLE, /**< The color purple. */
    LIGHT_BLUE, /**< The color light blue. */
    PINK, /**< The color pink. */
    ORANGE, /**< The color orange. */
    RED, /**< The color red. */
    YELLOW, /**< The color yellow. */
    GREEN, /**< The color green. */
    BLUE /**< The color blue. */
};

/**
 * @enum PropertyRent
 * @brief Represents the different rent values for a property.
 * 
 * The PropertyRent enum class defines the different rent values that a property can have.
 * These values are used to determine the rent amount when a player lands on a property.
 * The rent values include FULL_GROUP, ONE_HOUSE, TWO_HOUSES, THREE_HOUSES, FOUR_HOUSES, HOTEL, and BASE_RENT.
 */
enum class PropertyRent
{
    FULL_GROUP, /**< The rent value when the player owns the full group of properties. */
    ONE_HOUSE, /**< The rent value when the player owns one house on the property. */
    TWO_HOUSES, /**< The rent value when the player owns two houses on the property. */
    THREE_HOUSES, /**< The rent value when the player owns three houses on the property. */
    FOUR_HOUSES, /**< The rent value when the player owns four houses on the property. */
    HOTEL, /**< The rent value when the player owns a hotel on the property. */
    BASE_RENT /**< The base rent value of the property. */
};

/**
 * @brief The Property class represents a property space on the game board.
 *        It is a subclass of the BuyableSpace class.
 */
class Property : public BuyableSpace
{
    private:
        PropertyRent nbBuildings; /**< The number of buildings on the property. */
        Color color; /**< The color of the property. */
        int housePrice; /**< The price of a house on the property. */

    public:
        /**
         * @brief Constructs a Property object with the specified name, color, price, house price, and rent values.
         * @param _name The name of the property.
         * @param _color The color of the property.
         * @param _price The price of the property.
         * @param _housePrice The price of a house on the property.
         * @param _rent The rent values for the property.
         */
        Property(std::string _name, Color _color, int _price, int _housePrice, std::vector<int> _rent);

        /**
         * @brief Destructor for the Property class.
         */
        ~Property();

        /**
         * @brief Gets the price of a house on the property.
         * @return The house price.
         */
        int getHousePrice() const;

        /**
         * @brief Gets the color of the property.
         * @return The color of the property.
         */
        Color getColor() const;

        /**
         * @brief Gets the color of the property as a string.
         * @return The color of the property as a string.
         */
        std::string getColorString() const;

        /**
         * @brief Gets the number of buildings on the property.
         * @return The number of buildings.
         */
        PropertyRent getNbBuildings() const;

        /**
         * @brief Sets the number of buildings on the property.
         * @param _nbBuildings The number of buildings to set.
         */
        void setNbBuildings(PropertyRent _nbBuildings); 

        /**
         * @brief Performs the action associated with landing on the property space.
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Performs an auction for the property space.
         * @param board A shared pointer to the BoardManager object.
         */
        void auction(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Overloaded stream insertion operator to output the Property object.
         * @param os The output stream.
         * @param property The Property object to output.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const Property& property);
};


/**
 * @brief The Station class represents a station space on the game board.
 * 
 * This class inherits from the BuyableSpace class and provides additional functionality
 * specific to station spaces.
 */
class Station : public BuyableSpace
{
    private:
        int price = 200; /**< The price of the station. */
    public:
        /**
         * @brief Constructs a new Station object with the given name.
         * 
         * @param _name The name of the station.
         */
        Station(std::string _name); 

        /**
         * @brief Destroys the Station object.
         */
        ~Station();

        /**
         * @brief Performs the action associated with landing on a station space.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Conducts an auction for the station space.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void auction(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Overloaded insertion operator to allow output of Station objects.
         * 
         * @param os The output stream.
         * @param station The Station object to be output.
         * @return std::ostream& The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const Station& station);
};

/**
 * @class Utility
 * @brief Represents a utility space on the board that can be bought by players.
 * @details Inherits from the BuyableSpace class and provides additional functionality specific to utility spaces.
 */
class Utility : public BuyableSpace
{
    private:
        int price = 150; /**< The price of the utility space. */

    public:
        /**
         * @brief Construct a new Utility object with the given name.
         * @param _name The name of the utility space.
         */
        Utility(std::string _name);

        /**
         * @brief Destroy the Utility object.
         */
        ~Utility();

        /**
         * @brief Perform the action associated with landing on this utility space.
         * @param board A shared pointer to the BoardManager instance.
         */
        void action(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Perform an auction for this utility space.
         * @param board A shared pointer to the BoardManager instance.
         */
        void auction(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Overloaded insertion operator to print the utility space.
         * @param os The output stream to write to.
         * @param utility The utility space to print.
         * @return std::ostream& The output stream after writing the utility space.
         */
        friend std::ostream& operator<<(std::ostream& os, const Utility& utility);
};

/**
 * @brief The Tax class represents a space on the board that represents a tax.
 * 
 * It is a derived class of the Space class.
 */
class Tax : public Space
{
    private:
        std::string name; /**< The name of the tax. */
        int amount; /**< The amount of the tax. */
    public:
        /**
         * @brief Constructs a new Tax object with the given name and amount.
         * 
         * @param _name The name of the tax.
         * @param _amount The amount of the tax.
         */
        Tax(std::string _name, int _amount);

        /**
         * @brief Destroys the Tax object.
         */
        ~Tax();

        /**
         * @brief Gets the amount of the tax.
         * 
         * @return The amount of the tax.
         */
        int getAmount() const;

        /**
         * @brief Performs the action associated with the tax.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Overloads the << operator to allow printing of the Tax object.
         * 
         * @param os The output stream.
         * @param tax The Tax object to be printed.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const Tax& tax);
};

/**
 * @brief The Jail class represents a jail space on the board.
 */
class Jail : public Space
{
    private:
        std::string name = "Jail"; /**< The name of the jail space. */
    public:
        /**
         * @brief Constructs a Jail object.
         */
        Jail();
        
        /**
         * @brief Destroys the Jail object.
         */
        ~Jail();
        
        /**
         * @brief Performs the action associated with landing on the Jail space.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;
        
        /**
         * @brief Overloads the << operator to allow outputting the Jail object to an output stream.
         * 
         * @param os The output stream object.
         * @param jail The Jail object to be outputted.
         * @return The output stream object.
         */
        friend std::ostream& operator<<(std::ostream& os, const Jail& jail);
};

/**
 * @class GoToJail
 * @brief Represents a space on the board where players go to jail.
 * 
 * This class inherits from the Space class and provides specific functionality for the "Go To Jail" space.
 */
class GoToJail : public Space
{
    private:
        std::string name = "Go To Jail"; /**< The name of the Go To Jail space. */
    public:
        /**
         * @brief Default constructor for GoToJail.
         */
        GoToJail();
        
        /**
         * @brief Destructor for GoToJail.
         */
        ~GoToJail();
        
        /**
         * @brief Performs the action associated with the GoToJail space.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;
        
        /**
         * @brief Overloaded stream insertion operator for GoToJail.
         * 
         * @param os The output stream.
         * @param goToJail The GoToJail object to be inserted into the stream.
         * @return The modified output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const GoToJail& goToJail);
};

/**
 * @brief The FreeParking class represents a space on the board called "Free Parking".
 * 
 * This class inherits from the Space class and provides an implementation for the action() method.
 */
class FreeParking : public Space
{
    private:
        std::string name = "Free Parking"; /**< The name of the Free Parking space. */
    public:
        /**
         * @brief Constructs a new FreeParking object.
         */
        FreeParking();
        
        /**
         * @brief Destroys the FreeParking object.
         */
        ~FreeParking();
        
        /**
         * @brief Performs the action associated with landing on the Free Parking space.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;
        
        /**
         * @brief Overloads the << operator to allow printing of FreeParking objects.
         * 
         * @param os The output stream to write to.
         * @param freeParking The FreeParking object to be printed.
         * @return std::ostream& The output stream after writing the FreeParking object.
         */
        friend std::ostream& operator<<(std::ostream& os, const FreeParking& freeParking);
};

/**
 * @brief The Go class represents the "Go" space in the game board.
 * 
 * This class inherits from the Space class and provides specific functionality for the "Go" space.
 */
class Go : public Space
{
    private:
        std::string name = "Go"; /**< The name of the Go space. */
    public:
        /**
         * @brief Constructs a new Go object.
         */
        Go();
        
        /**
         * @brief Destroys the Go object.
         */
        ~Go();
        
        /**
         * @brief Performs the action associated with the "Go" space.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;
        
        /**
         * @brief Overloads the << operator to allow printing of Go objects.
         * 
         * @param os The output stream.
         * @param go The Go object to be printed.
         * @return std::ostream& The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const Go& go);
};

/**
 * @brief The CommunityChest class represents a community chest space on the board.
 * 
 * This class inherits from the Space class and provides specific functionality for community chest spaces.
 */
class CommunityChest : public Space
{
    private:
        std::string name = "Community Chest"; /**< The name of the community chest space. */
    public:
        /**
         * @brief Constructs a new CommunityChest object.
         */
        CommunityChest();

        /**
         * @brief Destroys the CommunityChest object.
         */
        ~CommunityChest();

        /**
         * @brief Performs the action associated with landing on a community chest space.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Overloads the << operator to allow for easy printing of CommunityChest objects.
         * 
         * @param os The output stream object.
         * @param communityChest The CommunityChest object to be printed.
         * @return std::ostream& The output stream object.
         */
        friend std::ostream& operator<<(std::ostream& os, const CommunityChest& communityChest);
};

/**
 * @brief The Chance class represents a chance space on the board.
 * 
 * This class inherits from the Space class and provides an implementation for the action() method.
 */
class Chance : public Space
{
    private:
        std::string name = "Chance"; /**< The name of the chance space. */
    public:
        /**
         * @brief Constructs a new Chance object.
         */
        Chance();

        /**
         * @brief Destroys the Chance object.
         */
        ~Chance();

        /**
         * @brief Performs the action associated with the chance space.
         * 
         * @param board A shared pointer to the BoardManager object.
         */
        void action(std::shared_ptr<BoardManager> board) override;

        /**
         * @brief Overloads the << operator to allow printing of Chance objects.
         * 
         * @param os The output stream object.
         * @param chance The Chance object to be printed.
         * @return std::ostream& A reference to the output stream object.
         */
        friend std::ostream& operator<<(std::ostream& os, const Chance& chance);
};

#endif // SPACES_H