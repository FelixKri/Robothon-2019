#include <Arduino.h>
#include "MeAuriga.h"
#include "Wire.h"

MeStepper stepper_1(1);
MeStepper stepper_2(2);

void setup() {
  stepper_1.setMaxSpeed(3000);
  stepper_2.setMaxSpeed(3000);
  stepper_1.move(400);
  stepper_2.move(400);
  stepper_1.setSpeed(3000);
  stepper_2.setSpeed(3000);
 
}

void loop() {
  stepper_1.runSpeedToPosition();
  stepper_2.runSpeedToPosition();
}
