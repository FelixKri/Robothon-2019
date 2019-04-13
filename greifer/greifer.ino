#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>
#include <SoftwareSerial.h>

MeSmartServo servo(PORT5);

void setup() {
  servo.run(-25);
  delay(2000);
  servo.run(0);
}

void loop() {
  servo.run(25);
  delay(2000);
  servo.run(0);
  delay(2000);
  servo.run(-25);
  delay(2000);
}
