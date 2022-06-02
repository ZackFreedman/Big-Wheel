

// keyPress(i,switchStates[i],lastSwitchStates[i],debouncedSwitchStates[i],lastDebouncedSwitchStates[i]);
void keyPress(int i, bool state,bool prevState, bool dbState, bool preveDbState)
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
      case 3:// 3 key on my setup

        Keyboard.press(MODIFIERKEY_CTRL);
        Keyboard.press(KEY_BACKSLASH);
        Keyboard.release(KEY_BACKSLASH);
        Keyboard.release(MODIFIERKEY_CTRL);
        break;
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
