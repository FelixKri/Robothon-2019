#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>
#include <SoftwareSerial.h>

MeDCMotor greifer(PORT_3);

void setup() {
  greifer.run(-25);
  delay(2000);
  greifer.run(0);
}

void loop() {
  greifer.run(25);
  delay(2000);
  greifer.run(0);
  delay(2000);
  greifer.run(-25);
  delay(2000);
}
