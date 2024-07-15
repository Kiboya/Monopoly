/**
 * @file main.cc
 * @brief Main file for the Monopoly game
 * 
 */

#include "sources/gamecore.h"
#include <iostream>
#include "utils/iomanager.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h" 
#include "spdlog/sinks/stdout_color_sinks.h"

/**
 * @brief Create the Monopoly board
 * @return A vector of shared pointers to the spaces on the board
 */
std::vector<std::shared_ptr<Space>> createBoard()
{
    std::vector<std::shared_ptr<Space>> board;
    board.push_back(std::make_shared<Go>());
    board.push_back(std::make_shared<Property>("Boulevard de Belleville", Color::PURPLE, 60, 50, std::vector<int>({4, 10, 30, 90, 160, 250, 2})));
    board.push_back(std::make_shared<CommunityChest>());
    board.push_back(std::make_shared<Property>("Rue Lecourbe", Color::PURPLE, 60, 50, std::vector<int>({8, 20, 60, 180, 320, 450, 4})));
    board.push_back(std::make_shared<Tax>("Impôt sur le revenu", 200));
    board.push_back(std::make_shared<Station>("Gare Montparnasse"));
    board.push_back(std::make_shared<Property>("Avenue de Vaugirard", Color::LIGHT_BLUE, 100, 50, std::vector<int>({12, 30, 90, 270, 400, 550, 6})));
    board.push_back(std::make_shared<Chance>());
    board.push_back(std::make_shared<Property>("Rue de Courcelles", Color::LIGHT_BLUE, 100, 50, std::vector<int>({12, 30, 90, 270, 400, 550, 6})));
    board.push_back(std::make_shared<Property>("Avenue de la République", Color::LIGHT_BLUE, 120, 50, std::vector<int>({16, 40, 100, 300, 450, 600, 8})));
    board.push_back(std::make_shared<Jail>());
    board.push_back(std::make_shared<Property>("Boulevard de la Villette", Color::PINK, 140, 100, std::vector<int>({20, 50, 150, 450, 625, 750, 10})));
    board.push_back(std::make_shared<Utility>("Compagnie de distribution d'électricité"));
    board.push_back(std::make_shared<Property>("Avenue de Neuilly", Color::PINK, 140, 100, std::vector<int>({20, 50, 150, 450, 625, 750, 10})));
    board.push_back(std::make_shared<Property>("Rue de Paradis", Color::PINK, 160, 100, std::vector<int>({24, 60, 180, 500, 700, 900, 12})));
    board.push_back(std::make_shared<Station>("Gare de Lyon"));
    board.push_back(std::make_shared<Property>("Avenue Mozart", Color::ORANGE, 180, 100, std::vector<int>({28, 70, 200, 550, 750, 950, 14})));
    board.push_back(std::make_shared<CommunityChest>());
    board.push_back(std::make_shared<Property>("Boulevard Saint-Michel", Color::ORANGE, 180, 100, std::vector<int>({28, 70, 200, 550, 750, 950, 14})));
    board.push_back(std::make_shared<Property>("Place Pigalle", Color::ORANGE, 200, 100, std::vector<int>({32, 80, 220, 600, 800, 1000, 16})));
    board.push_back(std::make_shared<FreeParking>());
    board.push_back(std::make_shared<Property>("Avenue Matignon", Color::RED, 220, 150, std::vector<int>({36, 90, 250, 700, 875, 1050, 18})));
    board.push_back(std::make_shared<Chance>());
    board.push_back(std::make_shared<Property>("Boulevard Malesherbes", Color::RED, 220, 150, std::vector<int>({36, 90, 250, 700, 875, 1050, 18})));
    board.push_back(std::make_shared<Property>("Avenue Henri-Martin", Color::RED, 240, 150, std::vector<int>({40, 100, 300, 750, 925, 1100, 20})));
    board.push_back(std::make_shared<Station>("Gare du Nord"));
    board.push_back(std::make_shared<Property>("Faubourg Saint-Honoré", Color::YELLOW, 260, 150, std::vector<int>({44, 110, 330, 800, 975, 1150, 22})));
    board.push_back(std::make_shared<Property>("Place de la Bourse", Color::YELLOW, 260,  150, std::vector<int>({44, 110, 330, 800, 975, 1150, 22})));
    board.push_back(std::make_shared<Utility>("Compagnie des eaux"));
    board.push_back(std::make_shared<Property>("Rue La Fayette", Color::YELLOW, 280, 150, std::vector<int>({48, 120, 360, 850, 1025, 1200, 24})));
    board.push_back(std::make_shared<GoToJail>());
    board.push_back(std::make_shared<Property>("Avenue de Breteuil", Color::GREEN, 300, 200, std::vector<int>({52, 130, 390, 900, 1100, 1275, 26})));
    board.push_back(std::make_shared<Property>("Avenue Foch", Color::GREEN, 300, 200, std::vector<int>({52, 130, 390, 900, 1100, 1275, 26})));
    board.push_back(std::make_shared<CommunityChest>());
    board.push_back(std::make_shared<Property>("Boulevard des Capucines", Color::GREEN, 320, 200, std::vector<int>({56, 150, 450, 1000, 1200, 1400, 28})));
    board.push_back(std::make_shared<Station>("Gare Saint-Lazare"));
    board.push_back(std::make_shared<Chance>());  
    board.push_back(std::make_shared<Property>("Avenue des Champs-Élysées", Color::BLUE, 350, 200, std::vector<int>({70, 175, 500, 1100, 1300, 1500, 35})));
    board.push_back(std::make_shared<Tax>("Taxe de luxe", 100));
    board.push_back(std::make_shared<Property>("Rue de la Paix", Color::BLUE, 400, 200, std::vector<int>({100, 200, 600, 1400, 1700, 2000, 50})));
    return board;
}

/**
 * @brief Create the Community Chest deck
 * @return A vector of unique pointers to the Community Chest cards
 */
std::vector<std::unique_ptr<CommunityChestCard>> createCommunityChestDeck() {
    std::vector<std::unique_ptr<CommunityChestCard>> deck;

    deck.push_back(std::make_unique<CommunityChestCard> ("Get out of Jail Free. This card may be kept until needed",  
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            currentPlayer->setHasCommunityChestGOJFC(true);
            board->setCommunityChestGOJFCTaken(true);
        }));
    

    deck.push_back(std::make_unique<CommunityChestCard> ("Advance to Go", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 0 - currentPlayer->getPosition()); // +0 because the Go space is at index 0
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("Receive your annual income of 100€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 100);
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("Go to Jail. Do not pass Go. Do not collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            currentPlayer->setRemainingTurnsInJail(3);
            currentPlayer->setPosition(10);
            board->handleSpace();
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("You have won second prize in a beauty contest. Collect 10€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 10);
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("Go back to Belleville", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 1 - currentPlayer->getPosition()); // +1 because Belleville is at index 1
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("Pay a fine of 10€ or draw a Chance card", 
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
                spdlog::error("Invalid input");
            }
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("The sale of your stock earns you 50€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 50);
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("Pay the doctor's fee of 50€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 50);
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("Pay your hospital fee of 100€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 100);
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("Bank error in your favor. Collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 200);
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("Receive your 7% interest on loan: 25€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 25);
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("The contributions reimburse you the amount of 20€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 20);
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("You inherit 100€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 100);
        }));

    deck.push_back(std::make_unique<CommunityChestCard> ("Pay your Insurance Premium of 50€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 50);
        }));

    deck.push_back(std::make_unique<CommunityChestCard>("It's your birthday, each player must give you 10€",
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

    return deck;
}

/**
 * @brief Create the Chance deck
 * @return A vector of unique pointers to the Chance cards
 */
std::vector<std::unique_ptr<ChanceCard>> createChanceDeck()
{
    std::vector<std::unique_ptr<ChanceCard>> deck;
    deck.push_back(std::make_unique<ChanceCard>("Make repairs to all your houses. Pay 25€ for each house and 100€ for each hotel",
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
            spdlog::info("You own {} houses and {} hotels.", nbHouses, nbHotels);
            spdlog::info("You have to pay {}€.", nbHouses * 25 + nbHotels * 100);
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, nbHouses * 25 + nbHotels * 100);
        }));

    deck.push_back(std::make_unique<ChanceCard>("You won the crossword prize. Receive 100€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 100);
        }));

    deck.push_back(std::make_unique<ChanceCard>("Fine for drunkenness. Pay 20€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 20);
        }));

    deck.push_back(std::make_unique<ChanceCard>("Go to Avenue Henri-Martin. If you pass Go, collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 24 - currentPlayer->getPosition()); // +24 because Avenue Henri-Martin is at index 24
        }));

    deck.push_back(std::make_unique<ChanceCard>("The bank pays you a dividend of 50€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 50);
        }));

    deck.push_back(std::make_unique<ChanceCard>("Go to Lyon Station. If you pass Go, collect 200€",
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 15 - currentPlayer->getPosition()); // +15 because Lyon Station is at index 15
        }));

    deck.push_back(std::make_unique<ChanceCard>("Advance to Boulevard de la Villette. If you pass Go, collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 11 - currentPlayer->getPosition()); // +11 because Boulevard de la Villette is at index 11
        }));

    deck.push_back(std::make_unique<ChanceCard>("Go to Rue de la Paix", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 39 - currentPlayer->getPosition()); // +39 because Rue de la Paix is at index 39
        }));

    deck.push_back(std::make_unique<ChanceCard>("Get out of Jail Free. This card may be kept until needed", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            currentPlayer->setHasChanceGOJFC(true);
            board->setChanceGOJFCTaken(true);
        }));

    deck.push_back(std::make_unique<ChanceCard>("Go to Jail. Do not pass Go. Do not collect 200€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            currentPlayer->setRemainingTurnsInJail(3);
            currentPlayer->setPosition(10); // setPosition instead of movePlayer because we don't want to pass by the Go space
            board->handleSpace();
        }));

    deck.push_back(std::make_unique<ChanceCard>("Speeding fine. Pay 15€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 15);
        }));

    deck.push_back(std::make_unique<ChanceCard>("You are taxed for road repairs at a rate of 40€ per house and 115€ per hotel", 
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
            spdlog::info("You own {} houses and {} hotels.", nbHouses, nbHotels);
            spdlog::info("You have to pay {}€.", nbHouses * 25 + nbHotels * 100);
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, nbHouses * 40 + nbHotels * 115);
        }));

    deck.push_back(std::make_unique<ChanceCard>("Move back three spaces", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(-3);
        }));

    deck.push_back(std::make_unique<ChanceCard>("Your property and loan are paying off. You must receive 150€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(nullptr, currentPlayer, 150);
        }));

    deck.push_back(std::make_unique<ChanceCard>("Pay for tuition fees 150€", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->getPlayerManager()->transferMoneyFromTo(currentPlayer, nullptr, 150);
        }));
    
    deck.push_back(std::make_unique<ChanceCard>("Advance to Go", 
        [](std::shared_ptr<BoardManager> board){
            std::shared_ptr<Player> currentPlayer = board->getPlayerManager()->getCurrentPlayer();
            board->movePlayer(board->getBoard().size() + 0 - currentPlayer->getPosition()); // +0 because Go is at index 0
        }));

    return deck;
}


/**
 * @brief Main function
 * @return 0 if the program runs successfully
 */
int main()
{
    // Set up the logger (logging level and output format)
    try 
    {       
        auto console_logger = spdlog::stdout_color_mt("console");
        spdlog::set_default_logger(console_logger);
        spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
        #ifdef DEBUG
            spdlog::set_level(spdlog::level::debug);
        #else
            spdlog::set_level(spdlog::level::info);
        #endif
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
    std::vector<std::shared_ptr<Space>> board = createBoard();
    std::vector<std::unique_ptr<CommunityChestCard>> communityChestDeck = createCommunityChestDeck();
    std::vector<std::unique_ptr<ChanceCard>> chanceDeck = createChanceDeck();
    GameCore game(board, std::move(communityChestDeck), std::move(chanceDeck));
    game.startGame();
    return 0;
}   