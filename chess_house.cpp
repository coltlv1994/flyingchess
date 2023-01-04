#include <vector>
#include <iostream>
#include <functional>
#include "chess_enum.hpp"
#include "chess_plane.hpp"
#include "chess_house.hpp"
#include "chess_board.hpp"

House::House(Color c, Board &mb) : color(c), motherBoard(mb)
{
    tarmacPlanes = 4;
    runwayPlanes = 0;
    inAirPlanes = 0;
    landedPlanes = 0;
    traversePath = TRAVERSE_INDEX_BY_HOUSE[color];
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
    std::cout << "House color: " << color << " with dice " << dice << " now moves." << std::endl;

    if ((inAirPlanes + runwayPlanes == 0) && (dice != 6))
    {
        std::cout << "No plane can move." << std::endl;
        return MoveFinished; // No plane can move.
    }

    if (tarmacPlanes != 0 && dice == 6)
    {
        for (Plane &p : planeList)
        {
            if (p.getPlaneStatus() == Tarmac)
            {
                p.setPlaneStatus(Runway);
                tarmacPlanes -= 1;
                runwayPlanes += 1;
                std::cout << "Plane " << p.getPlaneId() << "of color " << color << " moves to runway. " << std::endl;
            }
        }
        return ContinueToRoll;
    }

    int planeToFly = determineWhichPlaneToFly(dice);

    // And call that plane to move n steps.
    Plane &p = planeList[planeToFly];
    int planeNextStep = p.move(dice);
    int planeStepSpaceIndex = traversePath[planeNextStep];

    // If plane has reached the destination, update information.
    if (planeNextStep == MAX_STEPS_TO_FINISH)
    {
        // Check if there is a winner.
        landedPlanes += 1;
        inAirPlanes -= 1;
        askBoardToRemovePlane(p.getColor(), p.getPlaneId(), p.getCurrentStep());
        if (landedPlanes == 4)
        {
            return Victorious;
        }
        if (dice == 6)
        {
            return ContinueToRoll;
        }
        else
        {
            return MoveFinished;
        }
    }
    else
    {
        // movement has been made but not finished
        // First, ask board to check collision
        askBoardToHandleCollision(color, planeStepSpaceIndex);

        int stepBeforeJump = p.getCurrentStep();

        // Same color may jump
        if (motherBoard.getSpaceColor(planeStepSpaceIndex) == p.getColor())
        {
            // the check if plane may jump
            bool spaceMayJump = motherBoard.getSpaceJumpStatus(planeStepSpaceIndex);
            bool spaceMayLongJump = motherBoard.getSpaceLongJumpStatus(planeStepSpaceIndex);
            int jumpDestination = motherBoard.getSpaceJumpDestination(planeStepSpaceIndex);
            int longJumpDestination = motherBoard.getSpaceLongJumpDestination(planeStepSpaceIndex);
            int longJumpCollisionSpace = motherBoard.getLongJumpCollisionSpace(planeStepSpaceIndex);

            if (spaceMayLongJump == true)
            {
                // Can long jump and it steps on the long-jump position
                p.setCurrentStep(planeNextStep);
                planeNextStep = p.move(12);
                planeStepSpaceIndex = traversePath[planeNextStep];
                std::cout << "Plane long jump. Color: " << color << std::endl;

                // Check collision
                askBoardToHandleCollision(color, longJumpCollisionSpace);
                askBoardToHandleCollision(color, longJumpDestination);

                // Plane may still jump
                // There is no logic issue since this function will return anyway.
                planeNextStep = p.move(4);
                planeStepSpaceIndex = traversePath[planeNextStep];

                // Check collision
                askBoardToHandleCollision(color, planeStepSpaceIndex);

                askBoardToRemovePlane(p.getColor(), p.getPlaneId(), traversePath[stepBeforeJump]);
                p.setCurrentStep(planeNextStep);
                askBoardToAddPlane(p.getColor(), p.getPlaneId(), planeStepSpaceIndex);
                if (dice == 6)
                {
                    return ContinueToRoll;
                }
                else
                {
                    return MoveFinished;
                }
            }

            if (spaceMayJump == true)
            {

                // can jump
                p.setCurrentStep(planeNextStep);
                planeNextStep = p.move(4);
                planeStepSpaceIndex = traversePath[planeNextStep];
                std::cout << "Plane jumps to: " << planeStepSpaceIndex << std::endl;

                // Check if new space can long jump
                spaceMayLongJump = motherBoard.getSpaceLongJumpStatus(planeStepSpaceIndex);

                if (spaceMayLongJump)
                {
                    // Can long jump
                    longJumpDestination = motherBoard.getSpaceLongJumpDestination(planeStepSpaceIndex);
                    longJumpCollisionSpace = motherBoard.getLongJumpCollisionSpace(planeStepSpaceIndex);
                    p.setCurrentStep(planeNextStep);
                    planeNextStep = p.move(12);
                    planeStepSpaceIndex = traversePath[planeNextStep];

                    // Check collision
                    askBoardToHandleCollision(color, longJumpCollisionSpace);
                    askBoardToHandleCollision(color, longJumpDestination);
                }

                askBoardToRemovePlane(p.getColor(), p.getPlaneId(), traversePath[stepBeforeJump]);
                p.setCurrentStep(planeNextStep);
                askBoardToAddPlane(p.getColor(), p.getPlaneId(), planeStepSpaceIndex);
                if (dice == 6)
                {
                    return ContinueToRoll;
                }
                else
                {
                    return MoveFinished;
                }
            }
        }

        if (p.getCurrentStep() < (MAX_STEPS_TO_FINISH - 1))
        {
            askBoardToRemovePlane(p.getColor(), p.getPlaneId(), traversePath[p.getCurrentStep()]);
        }

        p.setCurrentStep(planeNextStep);
        askBoardToAddPlane(p.getColor(), p.getPlaneId(), planeStepSpaceIndex);
        if (dice == 6)
        {
            return ContinueToRoll;
        }
        else
        {
            return MoveFinished;
        }
    }
}

bool House::notifyPlaneStatusChanged(bool hasArrived)
{
    if (hasArrived)
    {
        landedPlanes += 1;
        if (landedPlanes == 4)
        {
            return true; // winner
        }
        else
        {
            return false;
        }
    }
    else
    {
        // Plane status changed due to collision
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
        return false;
    }
}

int House::determineWhichPlaneToFly(int diceInput)
{
    /* This function will determine which plane to move
       Return value: 0-3, plane index
    */

    std::vector<std::reference_wrapper<Plane>> availablePlaneList;

    for (Plane &p : planeList)
    {
        switch (p.getPlaneStatus())
        {
        case Tarmac:
            if (diceInput == 6)
            {
                p.setPlaneStatus(Runway);
                tarmacPlanes -= 1;
                runwayPlanes += 1;
                std::cout << "Plane #" << p.getPlaneId() << " moves to runway." << std::endl;
                return p.getPlaneId();
            }
            continue;

        case Runway:
        case Inair:
            availablePlaneList.push_back(p);

        default:
            continue;
        }
    }

    Plane &p = availablePlaneList[0];

    if (p.getPlaneStatus() == Runway)
    {
        p.setPlaneStatus(Inair);
        runwayPlanes -= 1;
        inAirPlanes += 1;
        std::cout << "Plane #" << p.getPlaneId() << " takes off." << std::endl;
    }

    return p.getPlaneId();
}

bool House::askBoardToRemovePlane(Color c, int uId, int sIndex)
{
    return motherBoard.removePlaneFromSpace(c, uId, sIndex);
}

bool House::askBoardToAddPlane(Color c, int uId, int sIndex)
{
    return motherBoard.addPlaneToSpace(c, uId, sIndex);
}

bool House::askBoardToHandleCollision(Color c, int sIndex)
{
    motherBoard.handlePlaneCollision(c, sIndex);
    return false;
}

bool House::queryBoardSpaceMayJump(int sIndex)
{
    return motherBoard.getSpaceJumpStatus(sIndex);
}

bool House::queryBoardSpaceMayLongJump(int sIndex)
{
    return motherBoard.getSpaceLongJumpStatus(sIndex);
}

void House::planeCollisionHandle(int spaceIndex)
{
    for (Plane &p : planeList)
    {
        if (TRAVERSE_INDEX_BY_HOUSE[color][p.getCurrentStep()] == spaceIndex)
        {
            p.resetPlane(false);
        }
    }
    notifyPlaneStatusChanged(false);
}

void House::endgameStatusPrintout(void)
{
    std::cout << std::endl
              << "House with color: " << color << " status." << std::endl;
    for (Plane &p : planeList)
    {
        int planeStep = p.getCurrentStep() >= MAX_STEPS_TO_FINISH ? MAX_STEPS_TO_FINISH - 1 : p.getCurrentStep();
        std::cout << "Plane #" << p.getPlaneId() << ", step: " << planeStep << ", space: " << traversePath[planeStep];
        std::cout << ", status: " << p.getPlaneStatus() << std::endl;
    }
}