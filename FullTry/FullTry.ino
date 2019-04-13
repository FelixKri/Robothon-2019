#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// ---------------------- Global Var's ---------------------
#define MIN_DISTANCE 75

// ---------------------- Motors/Sensors -------------------
MeStepper left_wheel(PORT_1);
MeStepper right_wheel(PORT_2);

MeDCMotor greifer(PORT_3);

MeSmartServo servo(PORT5);

MeLimitSwitch limit_1(PORT_6, 1);
MeLimitSwitch limit_2(PORT_6, 2); 

MeGyro gyro(0, 0x69); // onboard

MeRGBLed rgbled_0(0, 12); // 1-12 LED's | 0=all | onboard

void setup() {

  Serial.begin(9600);
  Serial.println("LOL");

  servo.begin(115200);
  servo.assignDevIdRequest();
    
  left_wheel.setMaxSpeed(SPEED);
  right_wheel.setMaxSpeed(SPEED);
  left_wheel.setSpeed(SPEED);
  right_wheel.setSpeed(SPEED);
  rgbled_0.setpin(44);
  distGes = 0;
  gyro.begin();

  servo.run(-25);
  delay(2000);
  servo.run(0);
  
  setColors(0,0,0);
  
}

void loop() {
  search_egg();
  drive_to_egg();
  grabb_egg();
  drive_back();
}

void search_egg(){
  turn_angle(90);
  bool found_egg = false;
  while(!found_egg){
    while(getUS() > MIN_DISTANCE){
      turn_angle(gyro.getAngle(3)+1);
    }
    found_egg = true;
  }
  //else continue
}

void drive_to_egg(){
  while(5 < getUS() < 75){
    drive_straight(); //short distance
  }
  turn_and_reposition();
  if(!limit1.touched() && !limit2.touched()){
    drive_to_egg();
  }
  //else continue to next
}

void grabb_egg(){
  //lower arm
  mysmartservo.moveTo(1,90,30);
  delay(1000);
  mysmartservo.setInitAngle(1);
  delay(1000);
  //grab
  servo.run(25);
  delay(2000);
  servo.run(0);
  //rise arm
  mysmartservo.moveTo(1,-90,30);
  delay(1000);
  mysmartservo.setInitAngle(1);
  //lose

  //continue
}

void setColors(int r, int g, int b){
  rgbled_0.setColor(0,r,g,b);
  rgbled_0.show();
}

