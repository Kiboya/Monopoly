/**
 * @file cards_unittest.cc
 * @brief Unit tests for the cards classes
 * 
 */

#include <gtest/gtest.h>
#include "../sources/cards.h"


TEST(CommunityChestCard, Constructor)
{
    CommunityChestCard card("Avancez jusqu'à la case départ", nullptr);
    EXPECT_EQ(card.getDescription(), "Avancez jusqu'à la case départ");
}

TEST(ChanceCard, Constructor)
{
    ChanceCard card("La banque vous verse un dividende de 50€", nullptr);
    EXPECT_EQ(card.getDescription(), "La banque vous verse un dividende de 50€");
}


