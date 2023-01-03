#include <vector>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
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

bool Plane::getIsJumped(void)
{
    return isJumped;
}

bool Plane::getIsLongJumped(void)
{
    return isLongJumped;
}

void Plane::setIsJumped(bool b)
{
    isJumped = b;
}

void Plane::setIsLongJumped(bool b)
{
    isLongJumped = b;
}

void Plane::resetPlane(bool isFinished)
{
    currentStep = MAX_STEPS_TO_FINISH;
    isJumped = false;
    isLongJumped = false;

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
    // Get whole board spaces reference
    std::vector<BoardSpace> &boardSpaces = masterHouse.getMotherBoard().getWholeBoardSpaces();

    // Get spaces reference
    BoardSpace &originalSpace = boardSpaces[TRAVERSE_INDEX_BY_HOUSE[color][currentStep]];

    // initial move
    int destinationStep = currentStep + steps;
    if (destinationStep == MAX_STEPS_TO_FINISH - 1)
    {
        // Plane has arrived in the destination
        resetPlane(true);
        originalSpace.removePlane(*this);
        return MAX_STEPS_TO_FINISH;
    }

    if (destinationStep >= MAX_STEPS_TO_FINISH)
    {
        // Plane shall wrap back
        destinationStep = (2 * MAX_STEPS_TO_FINISH - destinationStep) - 2;
        BoardSpace &destinationSpace = boardSpaces[TRAVERSE_INDEX_BY_HOUSE[color][destinationStep]];
        originalSpace.removePlane(*this);
        planeWriteToBoardSpace(boardSpaces, destinationStep);
        return destinationStep;
    }

    // The plane will not arrive and will not wrap.

    // Remove plane from original space
    BoardSpace &destinationSpace = boardSpaces[TRAVERSE_INDEX_BY_HOUSE[color][destinationStep]];

    // Remove this plane from original space
    if (originalSpace.removePlane(*this) == false)
    {
        // very unexpected;
        std::cerr << "Cannot remove plane from original space! " << std::endl;
        quick_exit(ExitError_RemoveFromOriginalSpace); //
    }

    // Add plane to destination space
    // Have to detect collision
    BoardSpace::collisionProcess(destinationSpace, color);

    // Add this plane to space if colors does not match
    // which means we will not jump
    if (destinationSpace.getColor() != color)
    {
        planeWriteToBoardSpace(boardSpaces, destinationStep);
        return destinationStep;
    }

    // Plane may jump
    bool spaceCanJump = destinationSpace.getCanJump();
    bool spaceCanLongJump = destinationSpace.getCanLongJump();

    if (spaceCanLongJump == true)
    {
        planeLongJump(boardSpaces, destinationSpace);
        destinationStep += 12;
        BoardSpace &intermediateSpace = boardSpaces[TRAVERSE_INDEX_BY_HOUSE[color][destinationStep]];
        if (isJumped == false)
        {
            // still can jump
            planeJump(boardSpaces, intermediateSpace);
            // After this jump it will be final move
            destinationStep += 4;
        }
        else
        {
            // cannot jump anymore, write
            ;
        }
        planeWriteToBoardSpace(boardSpaces, destinationStep);
        return destinationStep;
    }

    // space cannot long jump
    if (spaceCanJump == true)
    {
        planeJump(boardSpaces, destinationSpace);
        destinationStep += 4;

        // need to check if new place can long-jump
        BoardSpace &intermediateSpace = boardSpaces[TRAVERSE_INDEX_BY_HOUSE[color][destinationStep]];
        if (intermediateSpace.getCanLongJump() == true)
        {
            planeLongJump(boardSpaces, intermediateSpace);
            destinationStep += 12;
        }
        else
        {
            // Cannot long jump; this "intermediateSpace" is now final.
            ;
        }

        planeWriteToBoardSpace(boardSpaces, destinationStep);
        return destinationStep;
    }
    else
    {
        // cannot jump and cannot long jump
        planeWriteToBoardSpace(boardSpaces, destinationStep);
        return destinationStep;
    }
}

void Plane::planeLongJump(std::vector<BoardSpace> &boardSpaces, BoardSpace &startPoint)
{
    BoardSpace::collisionProcess(boardSpaces[startPoint.getLongJumpCollision()], color);
    BoardSpace::collisionProcess(boardSpaces[startPoint.getLongJumpDestination()], color);

    isLongJumped = true;
}

void Plane::planeJump(std::vector<BoardSpace> &boardSpaces, BoardSpace &startPoint)
{
    BoardSpace::collisionProcess(boardSpaces[startPoint.getJumpDestination()], color);

    isJumped = true;
}

void Plane::planeWriteToBoardSpace(std::vector<BoardSpace> &boardSpaces, int stepIndex)
{
    BoardSpace &finalDestination = boardSpaces[TRAVERSE_INDEX_BY_HOUSE[color][stepIndex]];
    isLongJumped = false;
    isJumped = false;
    finalDestination.addPlane(*this);
}