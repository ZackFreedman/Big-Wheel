#define topKnobA 17
#define topKnobB 16
#define middleKnobA 11
#define middleKnobB 10
#define lowerKnobA 15
#define lowerKnobB 14
#define wheelA 0
#define wheelB 1
#define bigWheelButtonPin 9
#define bottomKnobButtonPin 12
#define middleKnobButtonPin 8
#define topKnobButtonPin 13


// To simplify wiring, keys are actually in two separate matrices.
// The Teensy has like a million GPIO's, I can sacrifice four.
#define leftButtonsA 23
#define leftButtonsB 22
#define leftButtonsC 18
#define leftButtons1 21
#define leftButtons2 20
#define leftButtons3 19

#define rightButtonsA 2
#define rightButtonsB 6
#define rightButtonsC 7
#define rightButtons1 3
#define rightButtons2 5
#define rightButtons3 4

const byte leftButtonRowPins[] = {leftButtonsA, leftButtonsB, leftButtonsC};
const byte rightButtonRowPins[] = {rightButtonsA, rightButtonsB, rightButtonsC};

const byte allButtonRowPins[] = {leftButtonsA, leftButtonsB, leftButtonsC,
                           rightButtonsA, rightButtonsB, rightButtonsC
                          };

const byte leftButtonColumnPins[] = {leftButtons1, leftButtons2, leftButtons3};
const byte rightButtonColumnPins[] = {rightButtons1, rightButtons2, rightButtons3};

const byte allButtonColumnPins[] = {leftButtons1, leftButtons2, leftButtons3,
                              rightButtons1, rightButtons2, rightButtons3
                             };


const byte keyMatrix[2][3][3] = {
  {
    {2, 1, 0},
    {8, 7, 11},
    {17, 12, 15}
  },
  {
    {5, 3, 4},
    {9, 16, 13},
    {6, 14, 10}
  }
};
