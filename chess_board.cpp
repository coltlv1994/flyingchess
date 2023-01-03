#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_board.hpp"

BoardSpace::BoardSpace(int sId, Color c, bool cj, int jd, bool clj, int ljd, int ljc, bool f)
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
    for (Plane &p : planeList)
    {
        p.resetPlane(false);
    }

    planeList.clear();
    planeColor = NoColor;
}

bool BoardSpace::addPlane(Plane &plane)
{
    planeList.push_back(plane);
    planeColor = plane.getColor();
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
                if (planeList.size() == 0)
                {
                    planeColor = NoColor;
                }
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

Color BoardSpace::getPlaneColor(void)
{
    return planeColor;
}

Board::Board(int ps, std::vector<BoardSpace> &bs) : playerCount(ps), randomSeed(time(NULL)), boardSpaces(bs)
{
    srand(randomSeed);
}

Board::Board(int ps, time_t rs, std::vector<BoardSpace> &bs) : playerCount(ps), randomSeed(rs), boardSpaces(bs)
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

BoardSpace &Board::getBoardSpaceByIndex(int index)
{
    return boardSpaces[index];
}

std::vector<BoardSpace> &Board::getWholeBoardSpaces(void)
{
    return boardSpaces;
}

void Board::gameInitialize(void)
{
    for (int i = 0; i < playerCount; i++)
    {
        houseList.emplace_back(static_cast<Color>(getRandomNumberZeroToThree()), this);
        houseList[i].initialHouse();
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

bool BoardSpace::collisionProcess(BoardSpace &space, Color incomingPlaneColor)
{
    Color spacePlaneColor = space.getPlaneColor();
    if (spacePlaneColor == NoColor || spacePlaneColor == incomingPlaneColor)
    {
        return false; // no collision happens
    }

    // collision happens
    space.removeAllPlanes();
    return true;
}