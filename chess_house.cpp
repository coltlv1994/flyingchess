#include <vector>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_board.hpp"
#include "chess_house.hpp"

House::House(Color c, Board &mb) : color(c), motherBoard(mb)
{
}

void House::initialHouse(void)
{
    for (int i = 0; i < MAX_PLANES_PER_HOUSE; i++)
    {
        planeList.emplace_back(color, i, this);
    }
}

bool House::isVictorious(void)
{
    if (landedPlanes == 4)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Board &House::getMotherBoard(void)
{
    return motherBoard;
}

HouseStatus House::diceFromBoard(int dice)
{
    if (tarmacPlanes == 4 && dice != 6)
    {
        return MoveFinished; // No plane can move.
    }

    if (tarmacPlanes == 4 && dice == 6)
    {
        // All planes in tarmac, one can move to runway and continue to roll.
        planeList[0].setPlaneStatus(Runway);
        tarmacPlanes -= 1;
        runwayPlanes += 1;
        return ContinueToRoll;
    }

    return MoveFinished;
}