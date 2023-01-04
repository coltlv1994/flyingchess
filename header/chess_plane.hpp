#ifndef _CHESS_PLANE_H_
#define _CHESS_PLANE_H_

#include <vector>
#include "chess_enum.hpp"

class Plane
{
private:
    const Color color;
    const int planeId;
    int currentStep = MAX_STEPS_TO_FINISH;
    PlaneStatus planeStatus = Tarmac;
    bool isJumped = false;
    bool isLongJumped = false;
    House &masterHouse; // A plane should be binded to a house
    void planeLongJump(std::vector<BoardSpace> &boardSpaces, BoardSpace &startPoint);
    void planeJump(std::vector<BoardSpace> &boardSpaces, BoardSpace &startPoint);
    void planeWriteToBoardSpace(std::vector<BoardSpace> &boardSpaces, int spaceIndex);

public:
    Plane(Color c, int pId, House &h);

    Color getColor(void);
    int getPlaneId(void);
    int getCurrentStep(void);
    void setCurrentStep(int cs);
    PlaneStatus getPlaneStatus(void);
    void setPlaneStatus(PlaneStatus ps);
    bool getIsJumped(void);
    bool getIsLongJumped(void);
    void setIsJumped(bool b);
    void setIsLongJumped(bool b);

    void resetPlane(bool isFinished);

    static bool isIdenticalPlane(Plane &a, Plane &b);

    // Return value is where the plane will be after move
    // MAX_STEPS means it has finished flying.
    int move(int n);

    void notifyMasterHouseUpdatePlanesStatus(void);
};

#endif