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
    time_t randomSeed = 0;
    if (argc >= 3)
    {
        playerCount = std::stoi(argv[1]);
        randomSeed = std::stoll(argv[2]);
        if (randomSeed == 0)
        {
            randomSeed = time(NULL);
        }
        Board newGameBoard = Board(playerCount, randomSeed);
        newGameBoard.gameInitialize();
        Color winningHouse = newGameBoard.gameRun();

        std::cout << "House of color " << winningHouse << " is winner!" << std::endl;
    }
    else
    {
        playerCount = std::stoi(argv[1]);
        Board newGameBoard = Board(playerCount);
        newGameBoard.gameInitialize();
        int winningHouse = newGameBoard.gameRun();
        std::cout << "House #" << winningHouse << " is winner!" << std::endl;
    }

    // Information about chess board spaces.

    return 0;
}
