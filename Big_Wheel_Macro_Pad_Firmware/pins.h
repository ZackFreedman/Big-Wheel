#define topKnobA 0
#define topKnobB 1
#define middleKnobA 17
#define middleKnobB 16
#define lowerKnobA 15
#define lowerKnobB 14
#define wheelA 11
#define wheelB 10

byte allKnobPins[] = {topKnobA, topKnobB,
                      middleKnobA, middleKnobB,
                      lowerKnobA, lowerKnobB,
                      wheelA, wheelB
                     };

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

byte leftButtonRowPins[] = {leftButtonsA, leftButtonsB, leftButtonsC};
byte rightButtonRowPins[] = {rightButtonsA, rightButtonsB, rightButtonsC};

byte allButtonRowPins[] = {leftButtonsA, leftButtonsB, leftButtonsC,
                           rightButtonsA, rightButtonsB, rightButtonsC
                          };

byte leftButtonColumnPins[] = {leftButtons1, leftButtons2, leftButtons3};
byte rightButtonColumnPins[] = {rightButtons1, rightButtons2, rightButtons3};

byte allButtonColumnPins[] = {leftButtons1, leftButtons2, leftButtons3,
                              rightButtons1, rightButtons2, rightButtons3
                             };
