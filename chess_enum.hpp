#ifndef CHESS_ENUM_H
#define CHESS_ENUM_H

// Color is defined in clock-wise order; THE SAME AS GAME ORDER
enum Color
{
    Blue,
    Green,
    Red,
    Yellow
};

enum PlaneStatus
{
    Tarmac, // in hanger
    Runway, // ready to move
    Inair,  // on the move
    Landed  // reached the destination
};

enum HouseStatus
{
    MoveFinished,
    ContinueToRoll,
    Victorious
};

const int MAX_SPACES = 76;

const int MAX_PLANES_PER_HOUSE = 4;

#endif