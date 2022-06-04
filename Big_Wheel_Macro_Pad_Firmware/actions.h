// These define special actions that take more than one keystroke
#define NO_ACTION 0x00
#define ACTION_SOURCE_MONITOR_WINDOW -1
#define ACTION_TIMELINES_WINDOW -2
#define ACTION_AUDIO_CLIP_MIXER_WINDOW -3
#define ACTION_UNDO -4
#define ACTION_JUMP_TO_IN_POINT -5
#define ACTION_CTRL_KEY -8

#define DEBUG

// Note that the special shuttling behavior isn't in this file, since it needs to be processed separately

// This maps matrix key positions onto their logical control ID's
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

 const int switchAssignments[] = {
  KEY_0, 
  KEY_1, 
  KEY_2, 
  KEY_3, // 3 key on my setup
  KEY_4, //1 key on my setup
  KEY_5, //2 key on my setup
  KEY_6, //down key on my setup
  KEY_7, 
  KEY_8, 
  KEY_9,
  KEY_A, //left key on my setup
  KEY_B, 
  KEY_C, 
  KEY_D, //up arrow on my setup
  KEY_E, // right arrow on my setup
  KEY_F, 
  KEY_G, //escape key on my setup
  KEY_H  // I wired the big knob click into the circuit, even though pressing it is physically impossible
  };

// CCW, CW topKnobAssignments middleKnobAssignments
const int middleKnobAssignments[] = {KEY_RIGHT_BRACE, KEY_LEFT_BRACE};
const int topKnobAssignments[] = {KEY_J, KEY_L};
const int lowerKnobAssignments[] = {KEY_DOWN, KEY_UP};
const int wheelAssignments[] = {KEY_RIGHT, KEY_LEFT}; 

// keyPress(i,switchStates[i],lastSwitchStates[i],debouncedSwitchStates[i],lastDebouncedSwitchStates[i]);
int keyPress(int i, bool state,bool prevState, bool dbState, bool preveDbState)
{
  //normal Keys
  if(state!=prevState){
    int key=-1;
    switch (i){
      case 0:
        key = KEY_0;
        break;
      case 1:
        key = KEY_1;
        break;
      case 2:
        key = KEY_2;
        break;
      case 3:// 3 key on my setup
        key = KEY_3;
        break;
      case 6://down key on my setup
        key = KEY_6;
        break;
      case 7:
        key = KEY_7;
        break;
      case 8:
        key = KEY_8;
        break;
      case 9:
        key = KEY_9;
        break;
      case 10://left key on my setup
        key = KEY_A;
        break;
      case 11:
        key = KEY_B;
        break;
      case 12:
        key = KEY_C;
        break;
      case 13://up arrow on my setup
        key = KEY_D;
        break;
      case 14:// right arrow on my setup
        key = KEY_E;
        break;
      case 15:
        key = KEY_F;
        break;
      case 16://escape key on my setup
        key = KEY_G;
        break;
      case 17:
        key = KEY_H;
        break;
      case 18:
        key = KEY_I;
        break;
      case 19:
        key = KEY_J;
        break;
      case 20:
        key = KEY_L;
        break;
      default:
        break;
    }
    if (state){
      Keyboard.press(key);
    }
    else{
      Keyboard.release(key);
    }
  }
  //rising edge
  if( dbState && state && (dbState!=preveDbState)){
    switch (i){
      case 4: //my button 1, i kinda arbutrarily placed my keys, so this may be different for you 
          Keyboard.press(MODIFIERKEY_SHIFT);
          Keyboard.press(KEY_LEFT_BRACE);
          Keyboard.release(KEY_LEFT_BRACE);
          Keyboard.release(MODIFIERKEY_SHIFT);
          #ifdef DEBUG_KEYS
            Serial.print("pressed: ");
            Serial.println("Shift + [");
          #endif
          break;
    case 5: //my button 2, i kinda arbutrarily placed my keys, so this may be different for you 
          Keyboard.press(MODIFIERKEY_SHIFT);
          Keyboard.press(KEY_RIGHT_BRACE);
          Keyboard.release(KEY_RIGHT_BRACE);
          Keyboard.release(MODIFIERKEY_SHIFT);
          #ifdef DEBUG_KEYS
            Serial.print("pressed: ");
            Serial.println("Shift + [");
          #endif
        break;
      case 21:
          Keyboard.press(KEY_K);
          Keyboard.release(KEY_K);
        break;
      default:
        break;
    }
  }
  //falling edge
  if( !dbState && !state && (dbState!=preveDbState)){
    switch (i){
      default:
        break;
    }
  }


}


void performAction(int action) {
  switch (action) {
    case NO_ACTION:
      break;
    case ACTION_SOURCE_MONITOR_WINDOW:
      Keyboard.press(MODIFIERKEY_SHIFT);
      Keyboard.press(KEY_2);
      Keyboard.release(KEY_2);
      Keyboard.release(MODIFIERKEY_SHIFT);
      break;
    case ACTION_TIMELINES_WINDOW:
      Keyboard.press(MODIFIERKEY_SHIFT);
      Keyboard.press(KEY_3);
      Keyboard.release(KEY_3);
      Keyboard.release(MODIFIERKEY_SHIFT);
      break;
    case ACTION_AUDIO_CLIP_MIXER_WINDOW:
      Keyboard.press(MODIFIERKEY_SHIFT);
      Keyboard.press(KEY_9);
      Keyboard.release(KEY_9);
      Keyboard.release(MODIFIERKEY_SHIFT);
      break;
    case ACTION_UNDO:
      Keyboard.press(MODIFIERKEY_GUI);  // I think I remapped this on my PC. Use MODIFIERKEY_CTRL if this doesn't work
      Keyboard.press(KEY_Z);
      Keyboard.release(KEY_Z);
      Keyboard.release(MODIFIERKEY_GUI);
      break;
    case ACTION_JUMP_TO_IN_POINT:
      Keyboard.press(MODIFIERKEY_SHIFT);
      Keyboard.press(KEY_I);
      Keyboard.release(KEY_I);
      Keyboard.release(MODIFIERKEY_SHIFT);
      break;
    case ACTION_CTRL_KEY:
      Keyboard.press(MODIFIERKEY_GUI);
      Serial.println("Ctrl down");
      break;
    default:
      Keyboard.press(action);
      Keyboard.release(action);
  }
}
