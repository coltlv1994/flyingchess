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
    int tarmacPlanes = 4;
    int runwayPlanes = 0;
    int inAirPlanes = 0;
    int landedPlanes = 0;

public:
    House(Color c, Board &mb);
    void initialHouse(void);
    bool isVictorious(void);
    Board &getMotherBoard(void);
    Color getHouseColor(void);

    HouseStatus diceFromBoard(int d);
    int determineWhichPlaneToFly(int diceInput);

    void notifyPlaneStatusChanged(void);

};

#endif