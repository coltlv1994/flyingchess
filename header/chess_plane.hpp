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
    House &masterHouse; // A plane should be binded to a house

public:
    Plane(Color c, int pId, House &h);
    Color getColor(void);
    int getPlaneId(void);
    int getCurrentStep(void);
    void setCurrentStep(int cs);
    PlaneStatus getPlaneStatus(void);
    void setPlaneStatus(PlaneStatus ps);

    void resetPlane(bool isFinished);

    static bool isIdenticalPlane(Plane &a, Plane &b);

    // Return value is where the plane will be after move
    // MAX_STEPS means it has finished flying.
    int move(int n);

    void notifyMasterHouseUpdatePlanesStatus(bool hasArrived);

    void notifyPlaneRemove(Color c, int uId, int spaceIndex);
};

#endif