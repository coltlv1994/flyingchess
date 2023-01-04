#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_house.hpp"
#include "chess_board.hpp"

bool boardSpacesInitialize(std::string filePath, std::vector<BoardSpace> &bs);

int main(int argc, char *argv[])
{
    int playerCount = 0;
    std::string boardConfigPath;
    if (argc == 3)
    {
        // first is executable path, second is player count, third is csv path
        boardConfigPath = argv[2];
        playerCount = std::stoi(argv[1]);
    }
    else
    {
        // Insufficient input, use default
        boardConfigPath = ".\\chessboard.csv";
        playerCount = 2;
    }

    // Information about chess board spaces.

    std::vector<BoardSpace> boardSpaces;
    boardSpacesInitialize(boardConfigPath, boardSpaces);

    Board newGameBoard = Board(playerCount, boardSpaces);
    newGameBoard.gameInitialize();
    int winningHouse = newGameBoard.gameRun();
    return 0;
}

bool boardSpacesInitialize(std::string filePath, std::vector<BoardSpace> &bs)
{
    std::ifstream boardConfig;
    boardConfig.open(filePath, std::ios::in);
    std::string line;

    if (boardConfig.is_open() == false)
    {
        return false;
    }

    while (std::getline(boardConfig, line))
    {
        std::stringstream ss(line);
        std::vector<int> vect;
        while (ss.good())
        {
            std::string substr;
            std::getline(ss, substr, ',');
            vect.push_back(std::stoi(substr));
        }

        bs.emplace_back(vect[0], static_cast<Color>(vect[1]), vect[2], vect[3], vect[4], vect[5], vect[6], vect[7]);
    }

    if (bs.size() != MAX_SPACES)
    {
        return false;
    }

    return true;
}