//Was copied from "Big Wheel" macro pad firmware
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


bool lastSwitchStates[18];
bool debouncedSwitchStates[18];
bool lastDebouncedSwitchStates[18];
elapsedMillis switchDebounceTimestamps[18];
#define switchDebounceTime 50
void initKeyMatrix(){
  for (int i = 0; i < 6; i++) {
    pinMode(allButtonRowPins[i], OUTPUT);
    digitalWrite(allButtonRowPins[i], HIGH);
    pinMode(allButtonColumnPins[i], INPUT_PULLUP);
  }
}

void handleMatrix(){
  bool switchStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (int i = 0; i < 3; i++) {
    digitalWrite(leftButtonRowPins[i], LOW);
    for (int j = 0; j < 3; j++) {
      if (!digitalRead(leftButtonColumnPins[j])) {
        switchStates[keyMatrix[0][i][j]] = true;
      }
    }
    digitalWrite(leftButtonRowPins[i], HIGH);

    digitalWrite(rightButtonRowPins[i], LOW);
    for (int j = 0; j < 3; j++) {
      if (!digitalRead(rightButtonColumnPins[j])) {
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
    keyPress(i, switchStates[i], lastSwitchStates[i], debouncedSwitchStates[i], lastDebouncedSwitchStates[i]);
    
  }
  for (int i = 0; i < 4; i++){
    int j = i+18;
    switchStates[j]=!digitalRead(standardButtons[i]);
    
    keyPress(j,switchStates[j],lastSwitchStates[j],switchStates[j],lastSwitchStates[j]);//fake debounce for now....

    //Serial.println(standardButtons[i];
  }

  if (!debouncedSwitchStates[15] && lastDebouncedSwitchStates[15]) {
    Keyboard.release(MODIFIERKEY_GUI);
    Serial.println("Ctrl up");
  }


  for (int i = 0; i < 18; i++)
    lastSwitchStates[i] = switchStates[i];
  
}
