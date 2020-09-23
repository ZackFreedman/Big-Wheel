// These define special actions that take more than one keystroke
#define NO_ACTION 0x00
#define ACTION_SOURCE_MONITOR_WINDOW -1
#define ACTION_TIMELINES_WINDOW -2
#define ACTION_AUDIO_CLIP_MIXER_WINDOW -3
#define ACTION_UNDO -4
#define ACTION_JUMP_TO_IN_POINT -5
#define ACTION_ALT_KEY -8

// Note that the special shuttling behavior isn't in this file, since it needs to be processed separately

// This maps Premiere shortcuts onto the physical keys.
const int keyAssignments[4][7] = {
  {NO_ACTION, KEY_F, KEY_Q, KEY_W, KEY_I, KEY_O, KEY_M},
  {ACTION_SOURCE_MONITOR_WINDOW, KEY_V, NO_ACTION, NO_ACTION, NO_ACTION, NO_ACTION, KEY_QUOTE},
  {ACTION_TIMELINES_WINDOW, KEY_A, NO_ACTION, NO_ACTION, NO_ACTION, NO_ACTION, KEY_COMMA},
  {NO_ACTION, ACTION_ALT_KEY, NO_ACTION, NO_ACTION, NO_ACTION, NO_ACTION, ACTION_UNDO}
};

// CCW, CW, Click
const int topKnobAssignments[] = {KEY_RIGHT_BRACE, KEY_LEFT_BRACE, ACTION_AUDIO_CLIP_MIXER_WINDOW};
const int middleKnobAssignments[] = {KEY_EQUAL, KEY_MINUS, KEY_BACKSLASH};
const int lowerKnobAssignments[] = {KEY_DOWN, KEY_UP, ACTION_JUMP_TO_IN_POINT};
// I wired the big knob click into the circuit, but the current design doesn't mechanically allow that.
const int wheelAssignments[] = {KEY_RIGHT, KEY_LEFT, NO_ACTION}; 

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
    case ACTION_ALT_KEY:
      Keyboard.press(MODIFIERKEY_ALT);
      Serial.println("Alt down");
      break;
    default:
      Keyboard.press(action);
      Keyboard.release(action);
  }
}

