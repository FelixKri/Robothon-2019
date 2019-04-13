#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// ---------------------- Global Var's ---------------------
#define MIN_DISTANCE 75
#define TURN_TO_THRESHOLD 2
// ---------------------- Motors/Sensors -------------------
int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2

int dirPin2 = mePort[PORT_2].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin2 = mePort[PORT_2].s2;//the Step pin connect to Base Board PORT1 SLOT2

MeDCMotor greifer(PORT_3);

MeSmartServo servo(PORT5);

MeUltrasonicSensor ultraSensor(PORT_7);


MeLimitSwitch limit_1(PORT_6, 1);
MeLimitSwitch limit_2(PORT_6, 2); 

MeGyro gyro(0, 0x69); // onboard

MeRGBLed rgbled_0(0, 12); // 1-12 LED's | 0=all | onboard

void setup() {

  Serial.begin(9600);
  Serial.println("LOL");

  servo.begin(115200);
  servo.assignDevIdRequest();
  
  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);

  pinMode(dirPin2, OUTPUT);
  pinMode(stpPin2, OUTPUT);
  rgbled_0.setpin(44);
  int distGes = 0;
  gyro.begin();

  greifer.run(50); //positive = close
  delay(2000);
  greifer.run(0);
  
  setColors(0,0,0);
  
}

void loop() {
  search_egg();
  drive_to_egg();
  grabb_egg();
  //drive_back();
}

void search_egg(){
  TurnTo(-90);
  bool found_egg = false;
  /*while(!found_egg){
    while(getUS() > MIN_DISTANCE){
      turn(1, 10);
    }
    found_egg = true;
  }*/

  int zAngle = gyro.getAngle(3);
  
  digitalWrite(dirPin,0);
  digitalWrite(dirPin2,0);
  while(!found_egg || zAngle>=90) {

    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800); 

    gyro.update();
    zAngle = gyro.getAngle(3);
    if(getUS() > MIN_DISTANCE)
    {
      found_egg=true;
    }
  }
  //else continue
}

void drive_to_egg(){
  //while(5 < getUS() < 75){
    //drive_straight(); //short distance
  //}
  //turn_and_reposition();
  //if(!limit1.touched() && !limit2.touched()){
  //  drive_to_egg();
  //}
  //else continue to next
}

void grabb_egg(){
  //open grabber
  greifer.run(-30);
  delay(3000);
  greifer.run(0);
  //lower arm
  //mysmartservo.moveTo(1,90,30);
  delay(1000);
//  mysmartservo.setInitAngle(1);
  delay(1000);
  //grab
  greifer.run(30);
  delay(3000);
  greifer.run(0);
  delay(1000);
  //rise arm
//  mysmartservo.moveTo(1,-90,30);
  delay(1000);
//  mysmartservo.setInitAngle(1);
  //open grabber
  greifer.run(-30);
  delay(3000);
  greifer.run(0);
  //continue
}

void setColors(int r, int g, int b){
  rgbled_0.setColor(0,r,g,b);
  rgbled_0.show();
}

//---------------------------------------- MOVEMENT ----------------------------
void moveStraight(boolean dir,int steps)
{
  digitalWrite(dirPin,dir);
  digitalWrite(dirPin2,1-dir);
  delay(50);
  for(int i=0;i<steps;i++)
  {
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800); 
  }
}
void turn (boolean dir, int steps){ //0=right, 1=left
  digitalWrite(dirPin,dir);
  digitalWrite(dirPin2,dir);
  delay(50);
  for(int i=0;i<steps;i++){
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800); 
  }
}
void TurnTo(int angle) {
  int zAngle = gyro.getAngle(3);
  digitalWrite(dirPin,zAngle<angle);
  digitalWrite(dirPin2,zAngle<angle);
  delay(50);
  while(abs (zAngle-angle)> TURN_TO_THRESHOLD) {
    Serial.println(zAngle);
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800); 

    gyro.update();
    zAngle = gyro.getAngle(3);
  }
}
double getUS()
{
  Serial.print("US ");
  Serial.println(ultraSensor.distanceCm());
  return ultraSensor.distanceCm();
}


