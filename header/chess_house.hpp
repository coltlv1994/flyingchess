#ifndef _CHESS_HOUSE_H_
#define _CHESS_HOUSE_H_

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"

class House
{
    // Controls players' logic

private:
    const Color color;
    std::vector<Plane> planeList;
    Board &motherBoard;
    int tarmacPlanes;
    int runwayPlanes;
    int inAirPlanes;
    int landedPlanes;
    const int *traversePath;
    bool queryBoardSpaceMayJump(int sIndex);
    bool queryBoardSpaceMayLongJump(int sIndex);
    HouseStrategy strategyIndex = MAX_NUMBER_OF_STRATEGY;

public:
    House(Color c, Board &mb);
    void initialHouse(void);
    bool isVictorious(void);
    Board &getMotherBoard(void);
    Color getHouseColor(void);

    HouseStatus diceFromBoard(int d);
    int determineWhichPlaneToFly(int diceInput);

    bool notifyPlaneStatusChanged(bool hasArrived);

    bool askBoardToRemovePlane(Color c, int uId, int sIndex);

    bool askBoardToAddPlane(Color c, int uId, int sIndex);

    bool askBoardToHandleCollision(Color c, int sIndex);

    void planeCollisionHandle(int spaceIndex);

    void endgameStatusPrintout(void);

    void setHouseStrategy(HouseStrategy hs);

    int determineWhichPlaneToFlyByJump(int diceInput);
};

#endif