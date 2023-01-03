#ifndef CHESS_ENUM_H
#define CHESS_ENUM_H

#include <vector>

// Color is defined in clock-wise order; THE SAME AS GAME ORDER
enum Color
{
    Blue,
    Green,
    Red,
    Yellow,
    NoColor
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

const int MAX_SPACES = 73;

const int MAX_STEPS_TO_FINISH = 56;

const int MAX_PLANES_PER_HOUSE = 4;

// HARDCODED
const int TRAVERSE_INDEX_BY_HOUSE[MAX_PLANES_PER_HOUSE][MAX_STEPS_TO_FINISH] = {
    // By color order

    // Blue
    { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,
      28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,57,58,59,60,61,72},

    // Green
    { 13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,
      38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,52,53,54,55,56,72},

    // Red
    { 26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,
      51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,62,63,64,65,66,72},

    // Yellow
    { 39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
      16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,67,68,69,70,71,72}};


enum programErrorOnExit
{
    ExitNormal = 0,
    ExitError_RemoveFromOriginalSpace
};
#endif