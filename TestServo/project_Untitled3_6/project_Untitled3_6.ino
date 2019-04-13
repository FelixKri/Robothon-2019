#include <Arduino.h>
#include <Wire.h>
#include <MeAuriga.h>

Servo servo_6_1;
MePort port_6(10);

void setup(){
  servo_6_1.attach(port_6.pin1());
  servo_6_1.write(0);  
}

void loop(){
    
}


