#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_house.hpp"

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
    std::vector<std::reference_wrapper<Plane>> planeList;

public:
    BoardSpace(Color c, int sId, bool cj, int jd, bool clj, int ljd, int ljc, bool f);
    void removeAllPlanes(void);
    bool addPlane(Plane &plane);
    bool removePlane(Plane &plane);
    bool isEmpty(void);

    Color getColor(void);
    int getSpaceId(void);
    bool getCanJump(void);
    int getJumpDestination(void);
    bool getCanLongJump(void);
    int getLongJumpDestination(void);
    int getLongJumpCollision(void);
    bool getIsFinal(void);
};

class Board
{
    // Controls the game's logic
    private:
    const int playerCount; // numbers of players
    const time_t randomSeed; // Seed of random number; can control the game.
    std::vector<House> houseList;
    int nextMoveHouseIndex = 0; // in previous list
    int getRandomNumberOneToSix(void); // roll the dice
    int getRandomNumberZeroToThree(void); // four houses with 4 planes

    public:
    Board(int ps); // start a new game
    Board(int ps, time_t rs); // start a new game with EXACT random seed.
    const static int R6 = RAND_MAX / 6 + 1; // RAND_MAX is (2^n - 1) which will not be divided by 6; +1 for accuracy
    const static int R4 = RAND_MAX / 4 + 2; // Same as above

    void gameInitialize(void); // initialize the game with given information
    int gameRun(void);
};

#endif