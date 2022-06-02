#define readInterval 100
elapsedMillis simpleTime;
const int standardButtons[4] = {topKnobButtonPin,//top button
                                middleKnobButtonPin,//middle knob
                                bottomKnobButtonPin,// 
                                bigWheelButtonPin};//big wheel

bool prevSimpleState[4]={0,0,0,0};
void setupSimpleButtons(){
  for (int i =0; i<4;i++){
    pinMode(standardButtons[i], INPUT_PULLUP);
  }
}


void handleButtons(){
  if(simpleTime>readInterval){
    for (int i = 0; i < 4; i++){
      int state=!digitalRead(standardButtons[i]);
      if(state&&(prevSimpleState[i]!=state)){
        Keyboard.press(keyAssignments[i]);
        Keyboard.release(keyAssignments[i]);
      }
      prevSimpleState[i]=state;
  
      //Serial.println(standardButtons[i];
    }
  }
}
