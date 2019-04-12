#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>

#define SPEED 3000
#define STEP 400
#define HALF_STEP 200

MeStepper left_wheel(1);
MeStepper right_wheel(2);

MeLimitSwitch limit_1(PORT_6, 1);
MeLimitSwitch limit_2(PORT_6, 2); 

MeGyro gyro(0, 0x69); // onboard

MeRGBLed rgbled_0(0, 12); // 1-12 LED's / 0=all

void setup() {

  Serial.begin(9600);
  
  left_wheel.setMaxSpeed(SPEED);
  right_wheel.setMaxSpeed(SPEED);
  rgbled_0.setpin(44);
  
  gyro.begin();
  
  rgbled_0.setColor(0,0,0,0);
  rgbled_0.show();
  
}

void loop() {
  rgbled_0.setColor(0,0,0,0);
  rgbled_0.show();
  while(!limit_1.touched() && !limit_2.touched()){
    //moveSteps(HALF_STEP, SPEED);
    turn_angle(90);
  }
  
  
}

void turn_angle(int angle){
  while(true) {
    gyro.update();
    Serial.println(gyro.getAngle(3));
    if(gyro.getAngle(3) > angle ){
      setColors(0,255,0);
    }
    if(gyro.getAngle(3) < angle ){
      setColors(0,0,255);
    }
  }
}


void moveSteps(int dist, int _speed){

  left_wheel.setSpeed(_speed);
  right_wheel.setSpeed(_speed);
  
  left_wheel.move(-dist);
  right_wheel.move(dist);

  while (left_wheel.run() || right_wheel.run()){
    left_wheel.runSpeedToPosition();
    right_wheel.runSpeedToPosition();
  }
}

void turn_right(int dist, int _speed){

  left_wheel.setSpeed(_speed);
  right_wheel.setSpeed(_speed);
  
  left_wheel.move(-dist);
  right_wheel.move(-dist);

  while (left_wheel.run() || right_wheel.run()){
    left_wheel.runSpeedToPosition();
    right_wheel.runSpeedToPosition();
  }
}

void turn_left(int dist, int _speed){

  left_wheel.setSpeed(_speed);
  right_wheel.setSpeed(_speed);
  
  left_wheel.move(dist);
  right_wheel.move(dist);

  while (left_wheel.run() || right_wheel.run()){
    left_wheel.runSpeedToPosition();
    right_wheel.runSpeedToPosition();
  }
}

void setColors(int r, int g, int b){
  rgbled_0.setColor(0,r,g,b);
  rgbled_0.show();
}
