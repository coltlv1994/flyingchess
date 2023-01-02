#include <vector>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_board.hpp"

Plane::Plane(Color c, int pId) : color(c), planeId(pId)
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

int Plane::getCurrentSpace(void)
{
    return currentSpace;
}

void Plane::setCurrentSpace(int cs)
{
    currentSpace = cs;
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
    currentSpace = MAX_SPACES;
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