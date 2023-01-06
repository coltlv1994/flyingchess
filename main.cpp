#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_house.hpp"
#include "chess_board.hpp"

bool boardSpacesInitialize(std::string filePath, std::vector<BoardSpace> &bs);

int main(int argc, char *argv[])
{
    int playerCount = 0;
    time_t totalGames = 0;
    if (argc == 6)
    {
        // debug mode for special seed
        playerCount = std::stoi(argv[2]);
        totalGames = std::stoll(argv[3]);

        Board newGameBoard = Board(playerCount, totalGames);

        newGameBoard.setTotalGames(1);
        newGameBoard.gameInitialize();
        std::string victoryStatus = newGameBoard.gameRun();

        std::cout << victoryStatus << std::endl;
    }
    else
    {
        // Simulation test with all random seed.
        playerCount = std::stoi(argv[1]);
        totalGames = std::stoll(argv[2]);
        Board newGameBoard = Board(playerCount, time(NULL));
        newGameBoard.setTotalGames(totalGames);
        newGameBoard.gameInitialize();
        std::string victoryStatus = newGameBoard.gameRun();

        std::cout << victoryStatus << std::endl;
    }

    // Information about chess board spaces.

    return 0;
}
