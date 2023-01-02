#ifndef CHESS_PLANE_H
#define CHESS_PLANE_H

#include <vector>
#include <iostream>
#include "chess_enum.hpp"

class Plane
{
private:
    const Color color;
    const int planeId;
    int currentSpace = MAX_SPACES;
    PlaneStatus planeStatus = Tarmac;
    bool isJumped = false;
    bool isLongJumped = false;

public:
    Plane(Color c, int pId);

    Color getColor(void);
    int getPlaneId(void);
    int getCurrentSpace(void);
    void setCurrentSpace(int cs);
    PlaneStatus getPlaneStatus(void);
    void setPlaneStatus(PlaneStatus ps);
    bool getIsJumped(void);
    bool getIsLongJumped(void);
    void setIsJumped(bool b);
    void setIsLongJumped(bool b);

    void resetPlane(bool isFinished);

    static bool isIdenticalPlane(Plane &a, Plane &b);
};

#endif