/**
 * @file gamecore_unittest.cc
 * @brief Unit tests for the gamecore
 * 
 */

#include <gtest/gtest.h>
#include "../sources/gamecore.h"
#include "../utils/iomanager.h"

/**
 * @brief Test case for the Dice class roll.
 * 
 * This test verifies that the roll of a dice is within the expected range.
 * If the roll is not within the expected range, an assertion failure is triggered.
 */
TEST(Dice, Roll)
{
    Dice dice;
    int roll = dice.roll();
    EXPECT_GE(roll, 1);
    EXPECT_LE(roll, 6);
}

/**
 * @brief Test case for the Dice class roll distribution.
 * 
 * This test verifies that the roll distribution of two dice is as expected.
 * It rolls the dice a specified number of times and checks the frequencies of the outcomes.
 * The expected frequencies are compared with the observed frequencies within a tolerance.
 * If any outcome differs significantly, an assertion failure is triggered.
 */
TEST(Dice, RollDistribution)
{
    const int nbRolls = 100000;
    const double tolerance = 0.05;

    std::vector<int> frequencies(11, 0); // 2 to 12 possible outcomes

    Dice dice1;
    Dice dice2;

    for (int i = 0; i < nbRolls; i++)
    {
        frequencies[dice1.roll() + dice2.roll() - 2]++;
    }

    std::vector<double> expectedFrequencies = {1.0/36, 2.0/36, 3.0/36, 4.0/36, 5.0/36, 6.0/36, 
                                               5.0/36, 4.0/36, 3.0/36, 2.0/36, 1.0/36}; 

    for (int i = 0; i < 11; i++){
        double observed = static_cast<double>(frequencies[i]) / nbRolls;
        double expected = expectedFrequencies[i];
        ASSERT_NEAR(observed, expected, tolerance * expected) << "Outcome " << (i + 2) << " differs significantly";
    }

}

/**
 * @brief Test case for a full game.
 * 
 * This test starts a game with a full board, a random number of players and plays until there is a winner or 1000 turns have passed.
 */
TEST(GameCore, StartGame)
{
    std::vector<std::shared_ptr<Space>> spaces;
    spaces.push_back(std::make_shared<Go>());
    spaces.push_back(std::make_shared<Property>("Boulevard de Belleville", Color::PURPLE, 60, 50, std::vector<int>({4, 10, 30, 90, 160, 250, 2})));
    spaces.push_back(std::make_shared<CommunityChest>());
    spaces.push_back(std::make_shared<Property>("Rue Lecourbe", Color::PURPLE, 60, 50, std::vector<int>({8, 20, 60, 180, 320, 450, 4})));
    spaces.push_back(std::make_shared<Tax>("Impôt sur le revenu", 200));
    spaces.push_back(std::make_shared<Station>("Gare Montparnasse"));
    spaces.push_back(std::make_shared<Property>("Avenue de Vaugirard", Color::LIGHT_BLUE, 100, 50, std::vector<int>({12, 30, 90, 270, 400, 550, 6})));
    spaces.push_back(std::make_shared<Chance>());
    spaces.push_back(std::make_shared<Property>("Rue de Courcelles", Color::LIGHT_BLUE, 100, 50, std::vector<int>({12, 30, 90, 270, 400, 550, 6})));
    spaces.push_back(std::make_shared<Property>("Avenue de la République", Color::LIGHT_BLUE, 120, 50, std::vector<int>({16, 40, 100, 300, 450, 600, 8})));
    spaces.push_back(std::make_shared<Jail>());
    spaces.push_back(std::make_shared<Property>("Boulevard de la Villette", Color::PINK, 140, 100, std::vector<int>({20, 50, 150, 450, 625, 750, 10})));
    spaces.push_back(std::make_shared<Utility>("Compagnie de distribution d'électricité"));
    spaces.push_back(std::make_shared<Property>("Avenue de Neuilly", Color::PINK, 140, 100, std::vector<int>({20, 50, 150, 450, 625, 750, 10})));
    spaces.push_back(std::make_shared<Property>("Rue de Paradis", Color::PINK, 160, 100, std::vector<int>({24, 60, 180, 500, 700, 900, 12})));
    spaces.push_back(std::make_shared<Station>("Gare de Lyon"));
    spaces.push_back(std::make_shared<Property>("Avenue Mozart", Color::ORANGE, 180, 100, std::vector<int>({28, 70, 200, 550, 750, 950, 14})));
    spaces.push_back(std::make_shared<CommunityChest>());
    spaces.push_back(std::make_shared<Property>("Boulevard Saint-Michel", Color::ORANGE, 180, 100, std::vector<int>({28, 70, 200, 550, 750, 950, 14})));
    spaces.push_back(std::make_shared<Property>("Place Pigalle", Color::ORANGE, 200, 100, std::vector<int>({32, 80, 220, 600, 800, 1000, 16})));
    spaces.push_back(std::make_shared<FreeParking>());
    spaces.push_back(std::make_shared<Property>("Avenue Matignon", Color::RED, 220, 150, std::vector<int>({36, 90, 250, 700, 875, 1050, 18})));
    spaces.push_back(std::make_shared<Chance>());
    spaces.push_back(std::make_shared<Property>("Boulevard Malesherbes", Color::RED, 220, 150, std::vector<int>({36, 90, 250, 700, 875, 1050, 18})));
    spaces.push_back(std::make_shared<Property>("Avenue Henri-Martin", Color::RED, 240, 150, std::vector<int>({40, 100, 300, 750, 925, 1100, 20})));
    spaces.push_back(std::make_shared<Station>("Gare du Nord"));
    spaces.push_back(std::make_shared<Property>("Faubourg Saint-Honoré", Color::YELLOW, 260, 150, std::vector<int>({44, 110, 330, 800, 975, 1150, 22})));
    spaces.push_back(std::make_shared<Property>("Place de la Bourse", Color::YELLOW, 260,  150, std::vector<int>({44, 110, 330, 800, 975, 1150, 22})));
    spaces.push_back(std::make_shared<Utility>("Compagnie des eaux"));
    spaces.push_back(std::make_shared<Property>("Rue La Fayette", Color::YELLOW, 280, 150, std::vector<int>({48, 120, 360, 850, 1025, 1200, 24})));
    spaces.push_back(std::make_shared<GoToJail>());
    spaces.push_back(std::make_shared<Property>("Avenue de Breteuil", Color::GREEN, 300, 200, std::vector<int>({52, 130, 390, 900, 1100, 1275, 26})));
    spaces.push_back(std::make_shared<Property>("Avenue Foch", Color::GREEN, 300, 200, std::vector<int>({52, 130, 390, 900, 1100, 1275, 26})));
    spaces.push_back(std::make_shared<CommunityChest>());
    spaces.push_back(std::make_shared<Property>("Boulevard des Capucines", Color::GREEN, 320, 200, std::vector<int>({56, 150, 450, 1000, 1200, 1400, 28})));
    spaces.push_back(std::make_shared<Station>("Gare Saint-Lazare"));
    spaces.push_back(std::make_shared<Chance>());  
    spaces.push_back(std::make_shared<Property>("Avenue des Champs-Élysées", Color::BLUE, 350, 200, std::vector<int>({70, 175, 500, 1100, 1300, 1500, 35})));
    spaces.push_back(std::make_shared<Tax>("Taxe de luxe", 100));
    spaces.push_back(std::make_shared<Property>("Rue de la Paix", Color::BLUE, 400, 200, std::vector<int>({100, 200, 600, 1400, 1700, 2000, 50})));

    std::vector<std::unique_ptr<CommunityChestCard>> communityChestCards;
    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Get out of Jail Free. This card may be kept until needed",  
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            currentPlayer->setHasCommunityChestGOJFC(true);
            board->setCommunityChestGOJFCTaken(true);
        }));
    

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Advance to Go", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 0 - currentPlayer->getPosition()); // +0 because the Go space is at index 0
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Receive your annual income of 100€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 100);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Go to Jail. Do not pass Go. Do not collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            currentPlayer->setRemainingTurnsInJail(3);
            currentPlayer->setPosition(10);
            board->handleSpace();
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("You have won second prize in a beauty contest. Collect 10€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 10);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Go back to Belleville", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 1 - currentPlayer->getPosition()); // +1 because Belleville is at index 1
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Pay a fine of 10€ or draw a Chance card", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            char choice = getYesNo("Do you want to draw a Chance card? (y/n)");
            if (choice == 'y')
            {
                board->drawChanceCard();
            }
            else if (choice == 'n')
            {
                board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 10);
            }
            else
            {
                spdlog::error("Invalid input. Please enter 'y' or 'n'.");
            }
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("The sale of your stock earns you 50€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 50);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Pay the doctor's fee of 50€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 50);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Pay your hospital fee of 100€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 100);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Bank error in your favor. Collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 200);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Receive your 7% interest on loan: 25€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 25);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("The contributions reimburse you the amount of 20€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 20);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("You inherit 100€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 100);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard> ("Pay your Insurance Premium of 50€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 50);
        }));

    communityChestCards.push_back(std::make_unique<CommunityChestCard>("It's your birthday, each player must give you 10€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            for (int i = 0; i < board->getPlayerManager()->getNbPlayers(); ++i)
            {
                std::shared_ptr<Player> player = board->getPlayerManager()->getPlayer(i);
                if (player != currentPlayer)
                {
                    board->getPlayerManager()->transferMoneyFromTo(player, currentPlayer, 10);
                }
            }
        }));


    std::vector<std::unique_ptr<ChanceCard>> chanceCards;
    chanceCards.push_back(std::make_unique<ChanceCard>("Make repairs to all your houses. Pay 25€ for each house and 100€ for each hotel",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            int nbHouses = 0;
            int nbHotels = 0;
            std::vector<std::shared_ptr<Property>> owned_groups = board->getOwnedGroups(currentPlayer);
            for (std::shared_ptr<Property> property : owned_groups){
                if ((property->getNbBuildings() > PropertyRent::FULL_GROUP) && (property->getNbBuildings() < PropertyRent::HOTEL)){
                    nbHouses += static_cast<int>(property->getNbBuildings());
                }
                else if (property->getNbBuildings() == PropertyRent::HOTEL){
                    nbHotels += 1;
                }
            }
            spdlog::info("You own {0} houses and {1} hotels.", nbHouses, nbHotels);
            spdlog::info("You have to pay {0}€.", nbHouses * 25 + nbHotels * 100);
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, nbHouses * 25 + nbHotels * 100);
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("You won the crossword prize. Receive 100€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 100);
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Fine for drunkenness. Pay 20€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 20);
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Go to Avenue Henri-Martin. If you pass Go, collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 24 - currentPlayer->getPosition()); // +24 because Avenue Henri-Martin is at index 24
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("The bank pays you a dividend of 50€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 50);
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Go to Lyon Station. If you pass Go, collect 200€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 15 - currentPlayer->getPosition()); // +15 because Lyon Station is at index 15
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Advance to Boulevard de la Villette. If you pass Go, collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 11 - currentPlayer->getPosition()); // +11 because Boulevard de la Villette is at index 11
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Go to Rue de la Paix", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 39 - currentPlayer->getPosition()); // +39 because Rue de la Paix is at index 39
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Get out of Jail Free. This card may be kept until needed", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            currentPlayer->setHasChanceGOJFC(true);
            board->setChanceGOJFCTaken(true);
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Go to Jail. Do not pass Go. Do not collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            currentPlayer->setRemainingTurnsInJail(3);
            currentPlayer->setPosition(10); // setPosition instead of movePlayer because we don't want to pass by the Go space
            board->handleSpace();
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Speeding fine. Pay 15€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 15);
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("You are taxed for road repairs at a rate of 40€ per house and 115€ per hotel", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            int nbHouses = 0;
            int nbHotels = 0;
            std::vector<std::shared_ptr<Property>> owned_groups = board->getOwnedGroups(currentPlayer);
            for (std::shared_ptr<Property> property : owned_groups){
                if ((property->getNbBuildings() > PropertyRent::FULL_GROUP) && (property->getNbBuildings() < PropertyRent::HOTEL)){
                    nbHouses += static_cast<int>(property->getNbBuildings());
                }
                else if (property->getNbBuildings() == PropertyRent::HOTEL){
                    nbHotels += 1;
                }
            }
            spdlog::info("You own {0} houses and {1} hotels.", nbHouses, nbHotels);
            spdlog::info("You have to pay {0}€.", nbHouses * 25 + nbHotels * 100);
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, nbHouses * 40 + nbHotels * 115);
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Move back three spaces", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(-3);
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Your property and loan are paying off. You must receive 150€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 150);
        }));

    chanceCards.push_back(std::make_unique<ChanceCard>("Pay for tuition fees 150€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 150);
        }));
    
    chanceCards.push_back(std::make_unique<ChanceCard>("Advance to Go", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 0 - currentPlayer->getPosition()); // +0 because Go is at index 0
        }));

    GameCore gameCore(spaces, std::move(communityChestCards), std::move(chanceCards));
    gameCore.startGame();
}