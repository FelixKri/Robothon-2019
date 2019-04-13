#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>


MeDCMotor greifer(PORT_3);

void setup() {
  //greifer.run(50);
  //delay(2000);
  //greifer.run(0);
}

void loop() {
  greifer.run(-30);
  delay(2000);
  greifer.run(0);
  delay(2000);
  greifer.run(30);
  delay(2000);
  greifer.run(0);
  delay(2000);
}
