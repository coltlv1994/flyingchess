#ifndef _CHESS_BOARD_H_
#define _CHESS_BOARD_H_

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"

class BoardSpace
{
private:
    const Color color;
    const int spaceId;
    const bool canJump;
    const int jumpDestination;
    const bool canLongJump;
    const int longJumpDestination;
    const int longJumpCollision;
    const bool isFinal;
    std::vector<int> planeIndexList;
    Color planeColor = NoColor;

public:
    BoardSpace(int sId, Color c, bool cj, int jd, bool clj, int ljd, int ljc, bool f);
    bool isEmpty(void);

    Color getColor(void);
    int getSpaceId(void);
    bool getCanJump(void);
    int getJumpDestination(void);
    bool getCanLongJump(void);
    int getLongJumpDestination(void);
    int getLongJumpCollision(void);
    bool getIsFinal(void);

    Color getPlaneColor(void); // will return planes color

    Color collisionProcess(Color incomingPlaneColor);

    bool removePlane(Color c, int uId);
    bool addPlane(Color c, int uId);

    bool notifyHousePlaneRemove(Color c, int uId);
};

class Board
{
    // Controls the game's logic
private:
    const int playerCount;                // numbers of players
    const time_t randomSeed;              // Seed of random number; can control the game.
    std::vector<House> houseList;         // House should bind to a board.
    int nextMoveHouseIndex = 0;           // in previous list
    std::vector<BoardSpace> boardSpaces;  // A board must be bind to a fixed patter of board spaces and it will be a copy
    int getRandomNumberOneToSix(void);    // roll the dice
    int getRandomNumberZeroToThree(void); // four houses with 4 planes
    std::string boardConfigFilePath;
    bool initializeBoardSpaces(void);

public:
    Board(int ps);                          // start a new game
    Board(int ps, time_t rs);               // start a new game with EXACT random seed.
    const static int R6 = RAND_MAX / 6 + 1; // RAND_MAX is (2^n - 1) which will not be divided by 6; +1 for accuracy
    const static int R4 = RAND_MAX / 4 + 2; // Same as above

    void gameInitialize(void); // initialize the game with given information
    Color gameRun(void);

    BoardSpace &getBoardSpaceByIndex(int index);
    std::vector<BoardSpace> &getWholeBoardSpaces(void);
    void announceWinner(int houseIndex);

    bool removePlaneFromSpace(Color c, int uId, int sIndex);
    bool addPlaneToSpace(Color c, int uId, int sIndex);

    void handlePlaneCollision(Color c, int spaceIndex);

    bool getSpaceJumpStatus(int sIndex);
    bool getSpaceLongJumpStatus(int sIndex);
    int getSpaceJumpDestination(int sIndex);
    int getSpaceLongJumpDestination(int sIndex);
    int getLongJumpCollisionSpace(int sIndex);
    Color getSpaceColor(int sIndex);

    
};

#endif