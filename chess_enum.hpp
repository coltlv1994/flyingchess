#ifndef _CHESS_ENUM_H_
#define _CHESS_ENUM_H_

// Color is defined in clock-wise order; THE SAME AS GAME ORDER
enum Color
{
  Blue,
  Green,
  Red,
  Yellow,
  NoColor
};

// const std::string colorStrings[4] = {"Blue", "Green", "Red", "Yellow"};

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

extern const int MAX_SPACES;

extern const int MAX_STEPS_TO_FINISH;

extern const int MAX_PLANES_PER_HOUSE;

// HARDCODED
extern const int TRAVERSE_INDEX_BY_HOUSE[4][56];

enum programErrorOnExit
{
  ExitNormal = 0,
  ExitError_RemoveFromOriginalSpace
};

class Plane;

class House;

class BoardSpace;

class Board;

#endif