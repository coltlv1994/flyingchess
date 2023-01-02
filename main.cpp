#include <iostream>
#include <vector>
#include <fstream>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_board.hpp"
#include "chess_house.hpp"

void boardInitialize(std::string filePath);

int main(int argc, char* argv[])
{
    return 0;
}

void boardInitialize()
{
    std::ifstream boardConfig;
    boardConfig.open();
}