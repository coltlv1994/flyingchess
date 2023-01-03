#ifndef CHESS_HOUSE_H
#define CHESS_HOUSE_H

#include <vector>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_board.hpp"

class House
{
    // Controls players' logic

private:
    const Color color;
    std::vector<Plane> planeList;
    Board &motherBoard;
    int tarmacPlanes = 4;
    int runwayPlanes = 0;
    int inAirPlanes = 0;
    int landedPlanes = 0;

public:
    House(Color c, Board &mb);
    void initialHouse(void);
    bool isVictorious(void);
    Board &getMotherBoard(void);

    HouseStatus diceFromBoard(int d);
};

#endif