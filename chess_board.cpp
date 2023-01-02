#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_board.hpp"

BoardSpace::BoardSpace(Color c, int sId, bool cj, int jd, bool clj, int ljd, int ljc, bool f)
    : color(c), spaceId(sId), canJump(cj), jumpDestination(jd),
      canLongJump(clj), longJumpDestination(ljd), longJumpCollision(ljc), isFinal(f)
{
}

Color BoardSpace::getColor(void)
{
    return color;
}

int BoardSpace::getSpaceId(void)
{
    return spaceId;
}

bool BoardSpace::getCanJump(void)
{
    return canJump;
}

int BoardSpace::getJumpDestination(void)
{
    return jumpDestination;
}

bool BoardSpace::getCanLongJump(void)
{
    return canLongJump;
}

int BoardSpace::getLongJumpDestination(void)
{
    return longJumpDestination;
}

int BoardSpace::getLongJumpCollision(void)
{
    return longJumpCollision;
}

bool BoardSpace::getIsFinal(void)
{
    return isFinal;
}

void BoardSpace::removeAllPlanes(void)
{
    planeList.clear();
}

bool BoardSpace::addPlane(Plane &plane)
{
    planeList.push_back(plane);
    return true;
}

bool BoardSpace::removePlane(Plane &plane)
{
    if (planeList.empty() == true)
    {
        return false;
    }
    else
    {

        int position = 0;
        for (Plane &p : planeList)
        {
            if (Plane::isIdenticalPlane(p, plane) == true)
            {
                planeList.erase(planeList.begin() + position);
                return true;
            }
            else
            {
                position += 1;
            }
        }
        return false;
    }
}

bool BoardSpace::isEmpty(void)
{
    return planeList.empty();
}

Board::Board(int ps) : playerCount(ps), randomSeed(time(NULL))
{
    srand(randomSeed);
}

Board::Board(int ps, time_t rs) : playerCount(ps), randomSeed(rs)
{
    srand(randomSeed);
}

int Board::getRandomNumberOneToSix(void)
{
    return (rand() / Board::R6 + 1);
}

int Board::getRandomNumberZeroToThree(void)
{
    return (rand() / Board::R4);
}

void Board::gameInitialize(void)
{
    for (int i = 0; i < playerCount; i++)
    {
        houseList.emplace_back(static_cast<Color>(getRandomNumberZeroToThree()));
    }
}

int Board::gameRun(void)
{
    int diceValue;
    HouseStatus status;
    while (true)
    {
        diceValue = getRandomNumberOneToSix();
        status = houseList[nextMoveHouseIndex].diceFromBoard(diceValue);
        if (status == Victorious)
        {
            return nextMoveHouseIndex; // declared winner
        }
        else
        {
            if (status == ContinueToRoll)
            {
                continue;
            }
            else
            {
                nextMoveHouseIndex = (nextMoveHouseIndex + 1) % playerCount;
            }
        }
    }
}