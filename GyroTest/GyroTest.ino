#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>

MeGyro gyro_0(0, 0x69);

MeRGBLed rgbled_0(0, 12);



void setup(){
    gyro_0.begin();
    rgbled_0.setpin(44);
    rgbled_0.setColor(0,0,0,0);
    rgbled_0.show();
}

void loop(){
  gyro_0.update();
  int angle = gyro_0.getAngle(3);
  if(angle > 0 && angle < 90){
    rgbled_0.setColor(0,0,100,0);
    rgbled_0.show();
  }if(angle > 90 && angle < 180) {
    rgbled_0.setColor(0,100,0,0);
    rgbled_0.show();
  }if(angle < 0 && angle > -90) {
    rgbled_0.setColor(0,0,0,100);
    rgbled_0.show();
  }if(angle < -90 && angle > -180) {
    rgbled_0.setColor(0,100,100,100);
    rgbled_0.show();
  }
  delay(50);
}


