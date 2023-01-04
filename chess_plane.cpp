#include <vector>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_house.hpp"
#include "chess_board.hpp"

Plane::Plane(Color c, int pId, House &h) : color(c), planeId(pId), masterHouse(h)
{
}

Color Plane::getColor(void)
{
    return color;
}

int Plane::getPlaneId(void)
{
    return planeId;
}

int Plane::getCurrentStep(void)
{
    return currentStep;
}

void Plane::setCurrentStep(int cs)
{
    currentStep = cs;
}

PlaneStatus Plane::getPlaneStatus(void)
{
    return planeStatus;
}

void Plane::setPlaneStatus(PlaneStatus ps)
{
    planeStatus = ps;
}

void Plane::resetPlane(bool isFinished)
{
    currentStep = MAX_STEPS_TO_FINISH;

    if (isFinished == true)
    {
        planeStatus = Landed;
    }
    else
    {
        planeStatus = Tarmac;
    }
}

bool Plane::isIdenticalPlane(Plane &a, Plane &b)
{
    if ((a.color == b.color) && (a.planeId == b.planeId))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Plane::move(int steps)
{
    const int *traversePath = TRAVERSE_INDEX_BY_HOUSE[color];
    std::cout << "Plane #" << planeId << " of color " << color << " start in step #" << currentStep;

    int destinationStep = 0;

    if (currentStep == MAX_STEPS_TO_FINISH)
    {
        // Special treatment for runway planes
        // Have to use (MAX - 1) for avoiding nullptr and seg. fault.
        currentStep -= 1;
        std::cout << ". Ready to take off." << std::endl;
        destinationStep = steps - 1;
    }
    else
    {
        std::cout << "; spaceId: " << traversePath[currentStep] << std::endl;
        destinationStep = currentStep + steps;
        if (destinationStep >= MAX_STEPS_TO_FINISH)
        {
            // Wrap
            destinationStep = (2 * MAX_STEPS_TO_FINISH - destinationStep) - 2;
        }
    }

    std::cout << "Current spaceId: " << traversePath[currentStep] << ", destination spaceId: " << traversePath[destinationStep] << "." << std::endl;

    // initial move
    if (destinationStep == MAX_STEPS_TO_FINISH - 1)
    {
        // Plane has arrived in the destination
        resetPlane(true);
        currentStep = MAX_STEPS_TO_FINISH;
        // masterHouse.notifyPlaneStatusChanged(true);
        return MAX_STEPS_TO_FINISH;
    }

    return destinationStep;
}

void Plane::notifyMasterHouseUpdatePlanesStatus(bool hasArrived)
{
    masterHouse.notifyPlaneStatusChanged(hasArrived);
}

void Plane::notifyPlaneRemove(Color c, int uId, int spaceIndex)
{
    masterHouse.askBoardToRemovePlane(c, uId, spaceIndex);
}