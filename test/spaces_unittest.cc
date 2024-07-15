/**
 * @file test_spaces.cpp
 * @brief Unit tests for the spaces classes
 * 
 * This file contains unit tests for the different types of spaces in a Monopoly game.
 * It tests the creation and getters of each type of space, including properties, stations, utilities, taxes, jail, go to jail, free parking, go, community chest, and chance.
 */

#include <gtest/gtest.h>
#include "../sources/spaces.h" 

// Test the creation of each type of space
TEST(Space, Creation) {
    Property property1 = Property("Boulevard de Belleville", Color::PURPLE, 60, 50, std::vector<int>({2, 4, 10, 30, 90, 160, 250}));
    Station station1 = Station("Gare Montparnasse");
    Utility utility1 = Utility("Compagnie de distribution d'électricité");
    Tax tax1 = Tax("Impôt sur le revenu", 200);
    Jail jail1 = Jail();
    GoToJail goToJail1 = GoToJail();
    FreeParking freeParking1 = FreeParking();
    Go go1 = Go();
    CommunityChest communityChest1 = CommunityChest();
    Chance chance1 = Chance();

    // Check if the objects are created
    EXPECT_NE(&property1, nullptr);
    EXPECT_NE(&station1, nullptr);
    EXPECT_NE(&utility1, nullptr);
    EXPECT_NE(&tax1, nullptr);
    EXPECT_NE(&jail1, nullptr);
    EXPECT_NE(&goToJail1, nullptr);
    EXPECT_NE(&freeParking1, nullptr);
    EXPECT_NE(&go1, nullptr);
    EXPECT_NE(&communityChest1, nullptr);
    EXPECT_NE(&chance1, nullptr);
}

// Test the getters of each type of space
TEST(Space, Getters) {
    std::vector<int> propertyRent = {0, 0, 0, 0, 0, 0};
    Property property1 = Property("Boulevard de Belleville", Color::PURPLE, 60, 50, std::vector<int>({2, 4, 10, 30, 90, 160, 250}));
    Station station1 = Station("Gare Montparnasse");
    Utility utility1 = Utility("Compagnie de distribution d'électricité");
    Tax tax1 = Tax("Impôt sur le revenu", 200);
    Jail jail1 = Jail();
    GoToJail goToJail1 = GoToJail();
    FreeParking freeParking1 = FreeParking();
    Go go1 = Go();
    CommunityChest communityChest1 = CommunityChest();
    Chance chance1 = Chance();

    // Check if the objects are created
    EXPECT_EQ(property1.getName(), "Boulevard de Belleville");
    EXPECT_EQ(station1.getName(), "Gare Montparnasse");
    EXPECT_EQ(utility1.getName(), "Compagnie de distribution d'électricité");
    EXPECT_EQ(tax1.getName(), "Impôt sur le revenu");
    EXPECT_EQ(jail1.getName(), "Jail");
    EXPECT_EQ(goToJail1.getName(), "Go To Jail");
    EXPECT_EQ(freeParking1.getName(), "Free Parking");
    EXPECT_EQ(go1.getName(), "Go");
    EXPECT_EQ(communityChest1.getName(), "Community Chest");
    EXPECT_EQ(chance1.getName(), "Chance");

    // Check if the rent vectors are not empty for properties and stations and empty for utilities
    EXPECT_NE(property1.getRent().size(), 0);
    EXPECT_NE(station1.getRent().size(), 0);
    EXPECT_EQ(utility1.getRent().size(), 0);

    // Check if the colors are correct
    EXPECT_EQ(property1.getColor(), Color::PURPLE);

    // Check if the amount is correct
    EXPECT_EQ(tax1.getAmount(), 200);

    // Check if the number of buildings is correct
    EXPECT_EQ(property1.getNbBuildings(), PropertyRent::BASE_RENT);
    property1.setNbBuildings(PropertyRent::ONE_HOUSE);
    EXPECT_EQ(property1.getNbBuildings(), PropertyRent::ONE_HOUSE);

    // Check if the price is correct
    EXPECT_EQ(station1.getPrice(), 200);
    EXPECT_EQ(utility1.getPrice(), 150);
    EXPECT_EQ(property1.getPrice(), 60);
}