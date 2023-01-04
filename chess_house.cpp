#include <vector>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_house.hpp"
#include "chess_board.hpp"

House::House(Color c, Board &mb) : color(c), motherBoard(mb)
{
}

void House::initialHouse(void)
{
    for (int i = 0; i < MAX_PLANES_PER_HOUSE; i++)
    {
        planeList.emplace_back(color, i, *this);
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

Color House::getHouseColor(void)
{
    return color;
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

    // Should be a function that determine which plane to fly
    int planeToFly = determineWhichPlaneToFly(dice);

    // And call that plane to move n steps.
    Plane &p = planeList[planeToFly];
    int planeStatus = p.move(dice);

    // If plane has reached the destination, update information.
    if (planeStatus == MAX_STEPS_TO_FINISH)
    {
        // Check if there is a winner.
        landedPlanes += 1;
        if (landedPlanes == 4)
        {
            return Victorious;
        }
    }

    return MoveFinished;
}

void House::notifyPlaneStatusChanged(void)
{
    // A collision happens and this house's plane is affected.
    tarmacPlanes = 0;
    inAirPlanes = 0;
    for (Plane &p : planeList)
    {
        if (p.getPlaneStatus() == Tarmac)
        {
            tarmacPlanes += 1;
        }

        if (p.getPlaneStatus() == Inair)
        {
            inAirPlanes += 1;
        }
    }
}

int House::determineWhichPlaneToFly(int diceInput)
{
    /* This function will determine which plane to move
       Return value: 0-4, plane index
    */

    std::vector<std::reference_wrapper<Plane>> availablePlaneList;

    for (Plane &p : planeList)
    {
        if (p.getPlaneStatus() == Tarmac || p.getPlaneStatus() == Landed)
        {
            continue;
        }
        else
        {
            availablePlaneList.push_back(p);
        }
    }

    Plane &p = availablePlaneList[0];

    if (p.getPlaneStatus() == Runway)
    {
        p.setPlaneStatus(Inair);
        runwayPlanes -= 1;
        inAirPlanes += 1;
    }

    return p.getPlaneId();
}