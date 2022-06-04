#include <Encoder.h>
#include "actions.h"
#include "keyBindings.h"
#include "pins.h"
#include "BigEncoder.h"
#include "auxEncoders.h"
#include "simpleButtons.h"
#include "keyMatrixHandler.h"



bool systemLocked=false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupSimpleButtons();//from "simpleButtons.h"
  initKeyMatrix();//from keyMatrixHandler.h
}

void loop() {
  // put your main code here, to run repeatedly:
  processBigWheel();//from "BigEncoder.h"
  readAuxEncoders();//from "auxEncoders.h"
  handleButtons();//from "simpleButtons.h"
  handleMatrix();
}
