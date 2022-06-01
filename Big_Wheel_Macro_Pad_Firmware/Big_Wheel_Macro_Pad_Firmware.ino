/*
   "Big Wheel" macro pad firmware
   Copyright Sept. 2020 Zack Freedman of Voidstar Lab
   Licensed Creative Commons 4.0 Noncommercial Share-Alike

   This code is designed to run on this hardware: https://github.com/ZackFreedman/Big-Wheel
   This was used in a YouTube video! Check it out! https://www.youtube.com/watch?v=72a85tWOJVY

   Intended for Teensy LC, but should also work on Teensy 3.X and maybe Teensy 4.0
   These hotkeys and shortcuts are designed to control Premiere Pro, but you do you fam

   REMEMBER: Tools --> USB Type --> Serial + Keyboard + Mouse + Joystick
*/


#include <Encoder.h>
#include "actions.h"
#include "pins.h"

//#define DEBUG_DUMP_EVENTS
//#define DEBUG_DUMP_MATRIX
//#define DEBUG_DUMP_WHEEL_SPEED
//#define DEBUG_PLOT_WHEEL_SPEED
#define DEBUG_SHUTTLE

Encoder topKnob(topKnobA, topKnobB);
Encoder middleKnob(middleKnobA, middleKnobB);
Encoder lowerKnob(lowerKnobA, lowerKnobB);
Encoder wheel(wheelA, wheelB);

long lastKnobPositions[4] = {0, 0, 0, 0};
int lastKnobDeltas[4];
bool lastSwitchStates[18];
bool debouncedSwitchStates[18];
bool lastDebouncedSwitchStates[18];
elapsedMillis switchDebounceTimestamps[18];
#define switchDebounceTime 50

int lastActions[6];
int allActionsThisFrame[6];
byte actionIndex = 0;

// Used to calculate rotational velocity of big wheel
// 1 detent = 4 pulses in the same direction
// 1 delta = Time between two successive detents, in ms
#define wheelDetentDeltaAverageCount 5  // More detents = smoother but laggier shuttling

// How many ms must elapse without wheel motion before resetting velocity measurements
// wheelTimeout must be > 1000.0 / max(abs(minimumDpsForReverse), minimumDpsForPlay)
#define wheelTimeout 50

// More detents = more predictable but laggier shuttling.
// minimumWheelDetentsToAverage must be <= wheelDetentDeltaAverageCount!
#define minimumWheelDetentsToAverage 2

elapsedMillis timeSinceLastWheelDetent;
long wheelDetentDeltas[wheelDetentDeltaAverageCount];
byte wheelDetentDeltaIndex;  // Used to treat wheelDetentDeltas as a circular buffer
float averageWheelDetentDelta;

// Shuttle states. In normal mode, one big knob detent (4 pulses) moves by 1 frame.
// In other states, a shuttle movement (Hotkeys J or K in Premiere) is in effect.
#define NO_STATE -255 // Shuttle state should never be this
#define STATE_TRIPLE_REVERSE -3
#define STATE_DOUBLE_REVERSE -2
#define STATE_REVERSE -1
#define STATE_NORMAL 0
#define STATE_PLAY 1
#define STATE_DOUBLE_PLAY 2
#define STATE_TRIPLE_PLAY 3

// Minimum speed the wheel must spin to engage a shuttle mode
// DPS = detents per second
// Positive = clockwise
#define minimumDpsForTripleReverse -40
#define minimumDpsForDoubleReverse -30
#define minimumDpsForReverse -20
#define minimumDpsForPlay 25
#define minimumDpsForDoubleSpeed 40
#define minimumDpsForTripleSpeed 50

// To engage a shuttle mode, user must keep turning wheel at the threshold speed,
// or a higher threshold speed, for this many ms.
// Increase to make shuttling more predictable but less responsive.
// No effect on dead stops - wheelTimeout always takes priority.
#define timeUntilSpeedTransition 200

int shuttleState = STATE_NORMAL;
int stateTransitioningInto = NO_STATE;
elapsedMillis speedTransitionTimer;

// Special behavior - turning wheel while holding ALT key (2, 1)
// locks the shuttle into the Play or Reverse state, making viewing more comfortable.
// Any wheel movement, even one detent, unlocks the shuttle and returns to the Normal state.
bool shuttleLocked = false;
elapsedMillis shuttleLockTimer;
#define shuttleLockout 200  //  Wheel movement is ignored for this many ms after locking the shuttle.

// Lock the system by holding ALT (2, 1) and pressing Undo (2, 6)
bool systemLocked = false;  // While the system is locked, no keystrokes are sent over USB.
bool lockKeysLatched = false;  // After locking/unlocking, both keys are disabled until both are released
elapsedMillis ctrlGuardTimer;
// When ALT is pressed, user might be locking system.
// Don't send ALT until this many ms have passed.
#define ctrlGracePeriod 50

// Basic event buffer to collect all applicable shortcut/hotkey actions and send appropriate keystrokes
// Up to 6 actions can be queued, because I don't think the user can realistically hit more.
void queueAction(int action) {
#ifdef DEBUG_DUMP_EVENTS
  Serial.print("Action: ");
  Serial.println(action);
#endif

  if (action != NO_ACTION && actionIndex < 6) {
    allActionsThisFrame[actionIndex] = action;
    actionIndex++;
  }
}

void setup() {
  Serial.begin(9600);

  // Prepare key matrix pins (including encoder clicks)
  // Note that the Encoder library prepares its own pins.
  for (int i = 0; i < 6; i++) {
    pinMode(allButtonRowPins[i], OUTPUT);
    digitalWrite(allButtonRowPins[i], HIGH);
    pinMode(allButtonColumnPins[i], INPUT_PULLUP);
  }
}

void loop() {
  long knobPositions[] = {topKnob.read(), middleKnob.read(), lowerKnob.read(), wheel.read()};
  int knobDeltas[] = {0, 0, 0, 0};
  bool switchStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 6; i++) allActionsThisFrame[i] = NO_ACTION;
  actionIndex = 0;

  // This is necessary to implement shuttle lock
  bool ctrlBeingHeld = debouncedSwitchStates[15];
  if (timeSinceLastWheelDetent >= wheelTimeout) {
    
#ifdef DEBUG_SHUTTLE
    if (averageWheelDetentDelta != 0)
      Serial.println("Resetting speedometer");
#endif

    for (int i = 0; i < wheelDetentDeltaAverageCount; i++)
      wheelDetentDeltas[i] = 0;
    wheelDetentDeltaIndex = 0;

    averageWheelDetentDelta = 0;

    if (!shuttleLocked && shuttleState != STATE_NORMAL) {
#ifdef DEBUG_SHUTTLE
      Serial.println("Stopping shuttle (timeout)");
#endif

      Keyboard.release(MODIFIERKEY_GUI);

      if (ctrlBeingHeld)
        Keyboard.press(MODIFIERKEY_GUI);

      shuttleState = STATE_NORMAL;
    }

    stateTransitioningInto = NO_STATE;
  }


  for (int i = 0; i < 3; i++) {
    digitalWrite(leftButtonRowPins[i], LOW);
    for (int j = 0; j < 3; j++) {
      if (!digitalRead(leftButtonColumnPins[j])) {
#ifdef DEBUG_DUMP_MATRIX
        Serial.print("Left: ");
        Serial.print(i);
        Serial.print(',');
        Serial.print(j);
        Serial.print(" (Button ");
        Serial.print(keyMatrix[0][i][j]);
        Serial.println(')');
#endif

        switchStates[keyMatrix[0][i][j]] = true;
      }
    }
    digitalWrite(leftButtonRowPins[i], HIGH);

    digitalWrite(rightButtonRowPins[i], LOW);
    for (int j = 0; j < 3; j++) {
      if (!digitalRead(rightButtonColumnPins[j])) {
#ifdef DEBUG_DUMP_MATRIX
        Serial.print("Right: ");
        Serial.print(i);
        Serial.print(',');
        Serial.print(j);
        Serial.print(" (Button ");
        Serial.print(keyMatrix[1][i][j]);
        Serial.println(')');
#endif

        switchStates[keyMatrix[1][i][j]] = true;
      }
    }
    digitalWrite(rightButtonRowPins[i], HIGH);
  }

  for (int i = 0; i < 18; i++) {
    if (switchStates[i] != lastSwitchStates[i]) {
      switchDebounceTimestamps[i] = 0;
    }

    if (switchDebounceTimestamps[i] >= switchDebounceTime) {
      lastDebouncedSwitchStates[i] = debouncedSwitchStates[i];
      debouncedSwitchStates[i] = switchStates[i];
    }
    keyPress(i,switchStates[i],lastSwitchStates[i],debouncedSwitchStates[i],lastDebouncedSwitchStates[i]);
  }
  if (!systemLocked) {
    for (int i = 0; i < 4; i++) {
      if (abs(knobPositions[i] - lastKnobPositions[i]) >= 4) {
#ifdef DEBUG_DUMP_EVENTS
        Serial.print("Knob ");
        Serial.print(i);
        Serial.print(" ");
        Serial.print(lastKnobPositions[i]);
        Serial.print(" --> ");
        Serial.print(knobPositions[i]);
        Serial.print(" ");
#endif

        if (knobPositions[i] > lastKnobPositions[i]) {
#ifdef DEBUG_DUMP_EVENTS
          Serial.println("CW");
#endif
          knobDeltas[i] = 1;
        }
        else {
#ifdef DEBUG_DUMP_EVENTS
          Serial.println("CCW");
#endif
          knobDeltas[i] = -1;
        }

        lastKnobPositions[i] = knobPositions[i];
      }
    }

    if (knobDeltas[3] != 0) {
      wheelDetentDeltas[wheelDetentDeltaIndex] =
        long(timeSinceLastWheelDetent) * knobDeltas[3];  // Positive: CW, Negative: CCW

      wheelDetentDeltaIndex = (wheelDetentDeltaIndex + 1) % wheelDetentDeltaAverageCount;  // Baby's first circular buffer

      float detentsAveraged = 0;  // I think this needs to be a float or the division op will be truncated
      long runningTotal = 0;
      for (int i = 0; i < wheelDetentDeltaAverageCount; i++) {
        if (wheelDetentDeltas[i] == 0) break;

        detentsAveraged++;
        runningTotal += wheelDetentDeltas[i];
      }

      if (detentsAveraged >= minimumWheelDetentsToAverage) {
        averageWheelDetentDelta = float(runningTotal) / detentsAveraged;

#ifdef DEBUG_DUMP_WHEEL_SPEED
        Serial.print(averageWheelDetentDelta);
        Serial.print("ms --> ");
        Serial.print(1000.0 / averageWheelDetentDelta);
        Serial.println("/sec");
#endif
      }

      timeSinceLastWheelDetent = 0;
    }

    for (int i = 0; i < 4; i++) {
      // CW
      if (knobDeltas[i] > 0) {
        switch (i) {
          case 0:
            queueAction(topKnobAssignments[0]);
            break;
          case 1:
            queueAction(middleKnobAssignments[0]);
            break;
          case 2:
            queueAction(lowerKnobAssignments[0]);
            break;
          case 3:
            if (shuttleState == STATE_NORMAL)
              queueAction(wheelAssignments[0]);
            break;
          default:
            break;
        }
      }
      // CCW
      else if (knobDeltas[i] < 0) {
        switch (i) {
          case 0:
            queueAction(topKnobAssignments[1]);
            break;
          case 1:
            queueAction(middleKnobAssignments[1]);
            break;
          case 2:
            queueAction(lowerKnobAssignments[1]);
            break;
          case 3:
            if (shuttleState == STATE_NORMAL)
              queueAction(wheelAssignments[1]);
            break;
          default:
            break;
        }
      }
    }

    if (debouncedSwitchStates[15]) {  // Lock shuttle if CTRL is held
      if (shuttleLockTimer > shuttleLockout && knobDeltas[3] != 0) {
        Keyboard.release(MODIFIERKEY_GUI);

        Serial.print("Knob position: ");
        Serial.print(knobPositions[3]);
        Serial.print(" Last position: ");
        Serial.println(lastKnobPositions[3]);

        if (knobDeltas[3] == 1) {
          if (shuttleState != STATE_PLAY) {
#ifdef DEBUG_SHUTTLE
            Serial.println("Locking shuttle in Play");
#endif

            shuttleState = STATE_PLAY;
            bigWheelActions(shuttleState,false);
            
          }
        }
        else if (knobDeltas[3] == -1) {
          if (shuttleState != STATE_REVERSE) {
#ifdef DEBUG_SHUTTLE
            Serial.println("Locking shuttle in Reverse");
#endif

            shuttleState = STATE_REVERSE;
            bigWheelActions(shuttleState,false);
          }
        }

        stateTransitioningInto = NO_STATE;
        shuttleLocked = true;
        shuttleLockTimer = 0;

        Keyboard.press(MODIFIERKEY_GUI);
      }
    }
    else {
      if (knobDeltas[3] != 0 && shuttleLocked) {
#ifdef DEBUG_SHUTTLE
        Serial.print("Shuttle unlocked");
#endif
        shuttleState = STATE_NORMAL;
        shuttleLocked = false;
      }

      int stateShuttleShouldBeIn = NO_STATE;

      if (averageWheelDetentDelta == 0) {  // If wheel is still, remain locked or return to frame-by-frame scrubbing
        if (shuttleLocked)
          stateShuttleShouldBeIn = shuttleState;
        else
          stateShuttleShouldBeIn = STATE_NORMAL;
      }
      else {  // Wheel is moving and ALT is up. Are we moving fast enough to shuttle?
        float clicksPerSecond = 1000.0 / averageWheelDetentDelta;

        if (clicksPerSecond >= minimumDpsForTripleSpeed)
          stateShuttleShouldBeIn = STATE_TRIPLE_PLAY;
        else if (clicksPerSecond >= minimumDpsForDoubleSpeed)
          stateShuttleShouldBeIn = STATE_DOUBLE_PLAY;
        else if (clicksPerSecond >= minimumDpsForPlay)
          stateShuttleShouldBeIn = STATE_PLAY;
        else if (clicksPerSecond <= minimumDpsForTripleReverse)
          stateShuttleShouldBeIn = STATE_TRIPLE_REVERSE;
        else if (clicksPerSecond <= minimumDpsForDoubleReverse)
          stateShuttleShouldBeIn = STATE_DOUBLE_REVERSE;
        else if (clicksPerSecond <= minimumDpsForReverse)
          stateShuttleShouldBeIn = STATE_REVERSE;
        else
          stateShuttleShouldBeIn = STATE_NORMAL;
      }

      if (shuttleState == stateShuttleShouldBeIn) {  // No need to change state - we're already there
        if (stateTransitioningInto != NO_STATE) {
#ifdef DEBUG_SHUTTLE
          Serial.println("Cancelling transition");
#endif

          speedTransitionTimer = 0;
        }
        stateTransitioningInto = NO_STATE;
      }

      // To switch into a shuttle state, the wheel needs to keep spinning
      // at the appropriate speed or faster for a period of time.
      else if (stateTransitioningInto == stateShuttleShouldBeIn ||
               (stateTransitioningInto < 0 && stateShuttleShouldBeIn < stateTransitioningInto) ||
               (stateTransitioningInto > 0 && stateShuttleShouldBeIn > stateTransitioningInto)) {
        if (speedTransitionTimer >= timeUntilSpeedTransition) {
          if (stateTransitioningInto == STATE_NORMAL) {
            // Any transition to normal mode (frame-by-frame scrubbing) happens immediately.
            // The user must be able to precisely control the playhead at a moment's notice.

#ifdef DEBUG_SHUTTLE
            Serial.println("Stopping shuttle (slowdown)");
#endif
          }
          else {
            if (stateTransitioningInto == NO_STATE) {
#ifdef DEBUG_SHUTTLE
              Serial.println("Tried to transition into NO_STATE");
#endif
            }
            else {
              // Successive presses of the J and L keys control shuttle speed in Premiere Pro.
              // Pressing L starts shuttling at normal speed, L again goes to double speed,
              // then J goes back to normal speed, then J twice more reverses
              byte shiftsToPerform = abs(shuttleState - stateTransitioningInto);
              bool shiftDirection = stateTransitioningInto > shuttleState;

#ifdef DEBUG_SHUTTLE
              Serial.print(shiftDirection ? ">>> " : "<<< ");
              Serial.print("Shifting shuttle speed ");
              Serial.print(shiftDirection ? "up " : "down ");
              Serial.print(shiftsToPerform);
              Serial.print(" times, from ");
              Serial.print(shuttleState);
              Serial.print(" to ");
              Serial.println(stateTransitioningInto);
#endif

              

              

              
            }
          }

          shuttleState = stateTransitioningInto;
          stateTransitioningInto = NO_STATE;
        }
      }
      else {
#ifdef DEBUG_SHUTTLE
        Serial.print("Starting transition to ");
        Serial.println(stateShuttleShouldBeIn);
#endif

        stateTransitioningInto = stateShuttleShouldBeIn;
        speedTransitionTimer = 0;
      }
    }

    if (timeSinceLastWheelDetent >= wheelTimeout) {
      for (int i = 0; i < wheelDetentDeltaAverageCount; i++)
        wheelDetentDeltas[i] = 0;
      wheelDetentDeltaIndex = 0;
    }

#ifdef DEBUG_PLOT_WHEEL_SPEED
    // Good for making your own thresholds for shuttling
    if (averageWheelDetentDelta == 0)
      Serial.println(0.0);
    else
      Serial.println(1000.0 / averageWheelDetentDelta);
#endif
  }


  if (!systemLocked) {
    for (int i = 0; i < 6; i++) {
      bool actionAlreadyDone = false;

      if (allActionsThisFrame[i] == NO_ACTION) continue;

      for (int j = 0; j < 6; j++) {
        if (lastActions[j] == allActionsThisFrame[i]) {
          actionAlreadyDone = true;
          break;
        }
      }

      if (!actionAlreadyDone) {
        performAction(allActionsThisFrame[i]);
      }
    }
  }

  if (!debouncedSwitchStates[15] && !debouncedSwitchStates[16])
    lockKeysLatched = false;

  if (debouncedSwitchStates[15] && debouncedSwitchStates[16] && !lockKeysLatched) {
    Keyboard.releaseAll();

    for (int i = 0; i < 6; i++) {
      lastActions[i] = 0;
      allActionsThisFrame[i] = 0;
    }

    if (systemLocked) {
      Serial.println("System unlocked");
      systemLocked = false;
    }
    else {
      Serial.println("System locked");
      systemLocked = true;
    }

    lockKeysLatched = true;
  }

  for (int i = 0; i < 6; i++)
    lastActions[i] = allActionsThisFrame[i];

  for (int i = 0; i < 6; i++)
    lastKnobDeltas[i] = knobDeltas[i];

  for (int i = 0; i < 18; i++)
    lastSwitchStates[i] = switchStates[i];

#ifdef DEBUG_PLOT_WHEEL_SPEED
  // Don't kick computer in the crotch with firehose of USB traffic
  delay(10);
#endif
}
