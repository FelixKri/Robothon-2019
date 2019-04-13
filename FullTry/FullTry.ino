#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>

#define SPEED 3000
#define STEP 200
#define SMALL_STEP 5

MeStepper left_wheel(PORT_1);
MeStepper right_wheel(PORT_2);

//MeLimitSwitch limit_1(PORT_6, 1);
//MeLimitSwitch limit_2(PORT_6, 2); 

MeGyro gyro(0, 0x69); // onboard

MeRGBLed rgbled_0(0, 12); // 1-12 LED's / 0=all

void setup() {

  Serial.begin(9600);
  Serial.println("LOL");
  
  left_wheel.setMaxSpeed(SPEED);
  right_wheel.setMaxSpeed(SPEED);
  left_wheel.setSpeed(SPEED);
  right_wheel.setSpeed(SPEED);
  rgbled_0.setpin(44);
  distGes = 0;
  gyro.begin();
  
  setColors(0,0,0);
  
}

void loop() {
  moveSteps(200);
}

void turn_angle(int angle){
  while(abs(gyro.getAngle(3) - angle) < 1) {
    gyro.update();
    if(gyro.getAngle(3) > angle ){
      turn_left(SMALL_STEP);
      setColors(0,255,0);
    }
    if(gyro.getAngle(3) < angle ){
      turn_right(SMALL_STEP);
      setColors(0,0,255);
    }
  }
}

while (distGes <= 1000){
  void moveSteps(int dist){ 
    left_wheel.move(- dist);
    right_wheel.move(dist);
    distGes += Dist
  }
  
  
  void Step(){
    left_wheel.runSpeedToPosition();
    right_wheel.runSpeedToPosition();
  }
}

void turn_right(int dist){

  left_wheel.setSpeed(SPEED);
  right_wheel.setSpeed(SPEED);
  
  left_wheel.move(-dist);
  right_wheel.move(-dist);

  while (left_wheel.run() || right_wheel.run()){
    left_wheel.runSpeedToPosition();
    right_wheel.runSpeedToPosition();
  }
}

void turn_left(int dist){

  left_wheel.setSpeed(SPEED);
  right_wheel.setSpeed(SPEED);
  
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

