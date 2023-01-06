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
                std::cout << "Plane " << p.getPlaneId() << " of color " << color << " moves to runway. " << std::endl;
                break;
            }
        }
        std::cout << std::endl;
        return ContinueToRoll;
    }

    int planeToFly;
    // strategy comes here.
    if (strategyIndex == SeekJumpChance)
    {
        planeToFly = determineWhichPlaneToFlyByJump(dice);
    }
    else
    {
        planeToFly = determineWhichPlaneToFly(dice);
    }



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
        askBoardToRemovePlane(p.getColor(), p.getPlaneId(), traversePath[p.getCurrentStep()]);
        p.setPlaneStatus(Landed);
        p.setCurrentStep(MAX_STEPS_TO_FINISH);
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
                p.setCurrentStep(planeNextStep);
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
    if (inAirPlanes == 0)
    {
        for (Plane &p : planeList)
        {
            if (p.getPlaneStatus() == Runway)
            {
                p.setPlaneStatus(Inair);
                runwayPlanes -= 1;
                inAirPlanes += 1;
                return p.getPlaneId();
            }
        }
    }
    else
    {
        int farestStep = 0;
        int farestPlaneId = 0;
        for (Plane &p : planeList)
        {
            if (p.getPlaneStatus() == Inair)
            {
                if (p.getCurrentStep() > farestStep)
                {
                    farestPlaneId = p.getPlaneId();
                    farestStep = p.getCurrentStep();
                }
            }
        }
        return farestPlaneId;
    }
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
        if (p.getCurrentStep() == MAX_STEPS_TO_FINISH)
        {
            continue;
        }
        else
        {
            if (TRAVERSE_INDEX_BY_HOUSE[color][p.getCurrentStep()] == spaceIndex)
            {
                std::cout << "currentStep: " << p.getCurrentStep() << ", traverse space index: " << TRAVERSE_INDEX_BY_HOUSE[color][p.getCurrentStep()] << ", spaceIndex: " << spaceIndex << std::endl;
                p.resetPlane(false);
            }
        }
    }
    notifyPlaneStatusChanged(false);
}

void House::endgameStatusPrintout(void)
{
    std::cout << std::endl << "House with color: " << color << " status." << std::endl;
    for (Plane &p : planeList)
    {
        int planeStep = p.getCurrentStep() >= MAX_STEPS_TO_FINISH ? MAX_STEPS_TO_FINISH - 1 : p.getCurrentStep();
        std::cout << "Plane #" << p.getPlaneId() << ", step: " << planeStep << ", space: " << traversePath[planeStep];
        std::cout << ", status: " << p.getPlaneStatus() << std::endl;
    }
}

void House::setHouseStrategy(HouseStrategy hs)
{
    strategyIndex = hs;
}

int House::determineWhichPlaneToFlyByJump(int diceInput)
{
    /* This function seeks the plane with jump probability.
     */
    int planeJumpStepWithInput = 0; // MAX_STEPS means plane has no chance to jump.
    int planeStepAfterMove = 0;
    int planeMaxStepAfterJump = 0;
    int planeMaxJumpStep = 0;
    int planeMaxJumpId = 0;

    for (int i = 0; i < MAX_PLANES_PER_HOUSE; i++)
    {
        switch (planeList[i].getPlaneStatus())
        {
        case Tarmac:
        case Landed:
            continue;
        case Runway:
            if (diceInput == 2 || diceInput == 6)
            {
                planeJumpStepWithInput = 4;
                planeStepAfterMove = diceInput + 3;
            }
            else
            {
                planeJumpStepWithInput = 0;
                planeStepAfterMove = diceInput + 3;
            }
            break;
        case Inair:
            // here comes math
            // the board is completely symmetrical to all houses
            // so, step (4n + 1) may jump, and only step 17 may long jump
            planeStepAfterMove = planeList[i].getCurrentStep() + diceInput;
            if ((planeStepAfterMove % 4 != 1) || (planeStepAfterMove >= MAX_STEPS_TO_FINISH - 7))
            {
                // no jump possibility
            }
            else
            {
                switch (planeStepAfterMove)
                {
                    case 17:
                    case 13:
                        planeJumpStepWithInput = 16; // jump and long jump
                        planeStepAfterMove += planeJumpStepWithInput;
                        break;
                    default:
                        planeJumpStepWithInput = 4; // regular jump
                        planeStepAfterMove += planeJumpStepWithInput;
                        break;
                }
            }
        }

        if (planeJumpStepWithInput > planeMaxJumpStep)
        {
            planeMaxJumpStep = planeJumpStepWithInput;
            planeMaxStepAfterJump = planeStepAfterMove; // jump steps take priority
            planeMaxJumpId = i;
        }
        else
        {
            if (planeJumpStepWithInput == planeMaxJumpStep)
            {
                if (planeStepAfterMove > planeMaxStepAfterJump)
                {
                    planeMaxStepAfterJump = planeStepAfterMove; // farest takes priority
                    planeMaxJumpId = i;
                }
            }
        }
    }

    if (planeMaxJumpStep == 0)
    {
        return determineWhichPlaneToFly(diceInput); // fallback.
    }
    else
    {
        return planeMaxJumpId;
    }     
}