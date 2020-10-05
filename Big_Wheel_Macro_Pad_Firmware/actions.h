// These define special actions that take more than one keystroke
#define NO_ACTION 0x00
#define ACTION_SOURCE_MONITOR_WINDOW -1
#define ACTION_TIMELINES_WINDOW -2
#define ACTION_AUDIO_CLIP_MIXER_WINDOW -3
#define ACTION_UNDO -4
#define ACTION_JUMP_TO_IN_POINT -5
#define ACTION_CTRL_KEY -8

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

 const int switchAssignments[] = 
 {KEY_F, KEY_Q, KEY_W, KEY_I, KEY_O, 
 KEY_M, ACTION_AUDIO_CLIP_MIXER_WINDOW, ACTION_SOURCE_MONITOR_WINDOW, KEY_V, KEY_QUOTE,
 KEY_BACKSLASH, ACTION_TIMELINES_WINDOW, KEY_A, KEY_COMMA, ACTION_JUMP_TO_IN_POINT,
  ACTION_CTRL_KEY, ACTION_UNDO, 
  NO_ACTION  // I wired the big knob click into the circuit, even though pressing it is physically impossible
  };

// CCW, CW
const int topKnobAssignments[] = {KEY_RIGHT_BRACE, KEY_LEFT_BRACE};
const int middleKnobAssignments[] = {KEY_EQUAL, KEY_MINUS};
const int lowerKnobAssignments[] = {KEY_DOWN, KEY_UP};
const int wheelAssignments[] = {KEY_RIGHT, KEY_LEFT}; 

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

