#include <Arduino.h>
#include "MeAuriga.h"
#include "Wire.h"

MeStepper stepper_1(1);

void setup() {
  stepper_1.setMaxSpeed(3000);
  stepper_1.move(400);
  stepper_1.setSpeed(3000);
 
}

void loop() {
  stepper_1.runSpeedToPosition();
}
