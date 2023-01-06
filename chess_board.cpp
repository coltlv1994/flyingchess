#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <functional>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_house.hpp"
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

bool BoardSpace::isEmpty(void)
{
    return planeIndexList.empty();
}

Color BoardSpace::getPlaneColor(void)
{
    return planeColor;
}

Board::Board(int ps) : playerCount(ps), randomSeed(time(NULL))
{
    srand(randomSeed);
    boardConfigFilePath = ".\\chessboard.csv";
}

Board::Board(int ps, time_t rs) : playerCount(ps), randomSeed(rs)
{
    srand(randomSeed);
    boardConfigFilePath = ".\\chessboard.csv";
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
    return boardSpaces.at(index);
}

std::vector<BoardSpace> &Board::getWholeBoardSpaces(void)
{
    return boardSpaces;
}

void Board::gameInitialize(void)
{
    initializeBoardSpaces();
    std::cout << randomSeed << std::endl;
    int houseColorBitmap = 0;
    int houseColor = 0;
    int houseCount = 0;
    while (houseCount < playerCount)
    {
        houseColor = getRandomNumberZeroToThree();
        if (((1 << houseColor) & houseColorBitmap) == 0)
        {
            // which means the color is not taken
            houseList.emplace_back(static_cast<Color>(houseColor), *this);
            houseList[houseCount].initialHouse();
            houseColorBitmap |= (1 << houseColor);
            houseVictoryCount.push_back(0);
            houseCount += 1;
        }
    }
    houseList[0].setHouseStrategy(MoveTheFirst);
    houseList[1].setHouseStrategy(SeekJumpChance);
}

std::string Board::gameRun(void)
{
    int diceValue;
    HouseStatus status;
    while (gamesPlayed < totalGames)
    {
        
        diceValue = getRandomNumberOneToSix();
        std::cout << "House color: " << houseList[nextMoveHouseIndex].getHouseColor() << " moves with dice: " << diceValue << "." << std::endl;
        status = houseList[nextMoveHouseIndex].diceFromBoard(diceValue);
        std::cout << std::endl;
        if (status == Victorious)
        {
            houseVictoryCount[nextMoveHouseIndex] += 1;
            gamesPlayed += 1;
            boardResetForNextGame(0);
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

    std::cout << "House 0 wins " << houseVictoryCount[0] << " over " << houseVictoryCount[1] << " of house 1." << std::endl;

    return "\n";
}

void Board::announceWinner(int houseIndex)
{
    // std::cout << "Winner is: " << colorStrings[houseList[houseIndex].getHouseColor()] << std::endl;
}

Color BoardSpace::collisionProcess(Color incomingPlaneColor)
{
    if (planeColor == NoColor || planeColor == incomingPlaneColor)
    {
        return NoColor; // nothing happens
    }

    if (planeIndexList.empty() == true)
    {
        // nothing to remove
        return NoColor; // nothing happens
    }

    // collision happens
    std::cout << "Collision happens in space " << spaceId << ". Incoming plane color " << incomingPlaneColor << "; crash plane color: " << planeColor << "." << std::endl;
    planeIndexList.clear();

    Color returnValue = planeColor;
    planeColor = NoColor;

    return returnValue;
}

bool Board::removePlaneFromSpace(Color c, int uId, int sIndex)
{
    return boardSpaces[sIndex].removePlane(c, uId);
}

bool BoardSpace::removePlane(Color c, int uId)
{
    if (planeColor != c)
    {
        return false;
    }

    for (int &i : planeIndexList)
    {
        int position = 0;
        if (i == uId)
        {
            planeIndexList.erase(planeIndexList.begin() + position);
            if (planeIndexList.size() == 0)
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

bool BoardSpace::addPlane(Color c, int uId)
{
    if (planeColor != c && planeColor != NoColor)
    {
        return false;
    }

    if (std::find(planeIndexList.begin(), planeIndexList.end(), uId) != planeIndexList.end())
    {
        // Plane has existed, error
        return false;
    }
    else
    {
        planeIndexList.push_back(uId);
        planeColor = c;
        return true;
    }
}

void Board::handlePlaneCollision(Color c, int spaceIndex)
{
    // c: incoming plane's color
    // spaceIndex: BoardSpace need to handle collision
    // And notify a house to update its plane status

    Color collisionColor = boardSpaces[spaceIndex].collisionProcess(c);
    if (collisionColor != NoColor)
    {
        for (House &h : houseList)
        {
            if (h.getHouseColor() == collisionColor)
            {
                h.planeCollisionHandle(spaceIndex);
                return;
            }
        }
    }
}

bool Board::getSpaceJumpStatus(int sIndex)
{
    return boardSpaces[sIndex].getCanJump();
}

bool Board::getSpaceLongJumpStatus(int sIndex)
{
    return boardSpaces[sIndex].getCanLongJump();
}

int Board::getSpaceJumpDestination(int sIndex)
{
    return boardSpaces[sIndex].getJumpDestination();
}
int Board::getSpaceLongJumpDestination(int sIndex)
{
    return boardSpaces[sIndex].getLongJumpDestination();
}

int Board::getLongJumpCollisionSpace(int sIndex)
{
    return boardSpaces[sIndex].getLongJumpCollision();
}

Color Board::getSpaceColor(int sIndex)
{
    return boardSpaces[sIndex].getColor();
}

bool Board::addPlaneToSpace(Color c, int uId, int sIndex)
{
    return boardSpaces[sIndex].addPlane(c, uId);
}

bool Board::initializeBoardSpaces(void)
{
    boardSpaces.clear();
    std::ifstream boardConfig;
    boardConfig.open(boardConfigFilePath, std::ios::in);
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

        boardSpaces.emplace_back(vect[0], static_cast<Color>(vect[1]), vect[2], vect[3], vect[4], vect[5], vect[6], vect[7]);
    }

    if (boardSpaces.size() != MAX_SPACES)
    {
        return false;
    }

    std::cout << "Game board created." << std::endl;
    return true;
}

void Board::boardResetForNextGame(time_t newRandomSeed)
{
    randomSeed += 1;
    srand(randomSeed);
    houseList.clear();
    for (BoardSpace &bs : boardSpaces)
    {
        bs.resetBoardSpace();
    }

    // generate new house
    int houseColorBitmap = 0;
    int houseColor = 0;
    int houseCount = 0;
    while (houseCount < playerCount)
    {
        houseColor = getRandomNumberZeroToThree();
        if (((1 << houseColor) & houseColorBitmap) == 0)
        {
            // which means the color is not taken
            houseList.emplace_back(static_cast<Color>(houseColor), *this);
            houseList[houseCount].initialHouse();
            houseColorBitmap |= (1 << houseColor);
            houseCount += 1;
        }
    }

    houseList[0].setHouseStrategy(MoveTheFirst);
    houseList[1].setHouseStrategy(SeekJumpChance);

    nextMoveHouseIndex = 0;
}

void BoardSpace::resetBoardSpace(void)
{
    planeColor = NoColor;
    planeIndexList.clear();
}

void Board::setTotalGames(int gameCount)
{
    totalGames = gameCount;
}