#define timeBetweenReads 60

Encoder knobs[3]={Encoder(topKnobA, topKnobB),
                  Encoder(middleKnobA, middleKnobB),
                  Encoder(lowerKnobA, lowerKnobB)};


elapsedMillis auxEncoderTime;
long prevKnobPos[3];
void readAuxEncoders(){
  if(auxEncoderTime>=timeBetweenReads){
    long knobPositions[] = {knobs[0].read(), knobs[1].read(), knobs[2].read()};
    for(int i =0;i<3;i++){
      if (abs(knobPositions[i])>3){
        
        knobs[i].write(0);
        if(knobPositions[i]>0){
          Keyboard.press(knobAssignmets[i][1]);
          Keyboard.release(knobAssignmets[i][1]);
        }else{
          Keyboard.press(knobAssignmets[i][0]);
          Keyboard.release(knobAssignmets[i][0]);
        
        }
      }
    }
    

    auxEncoderTime=0;
  }
}
