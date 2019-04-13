#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// ---------------------- Global Var's ---------------------
#define MIN_DISTANCE 80
#define TURN_TO_THRESHOLD 2
bool FIRST_RUN = true;
int steps;
// ---------------------- Motors/Sensors -------------------
int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2

int dirPin2 = mePort[PORT_2].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin2 = mePort[PORT_2].s2;//the Step pin connect to Base Board PORT1 SLOT2

MeDCMotor greifer(PORT_3);

MeSmartServo mysmartservo(PORT5);

MeUltrasonicSensor ultraSensor(PORT_7);

MeLineFollower lineFinder(PORT_8);

MeLimitSwitch limit_1(PORT_6, 1);
MeLimitSwitch limit_2(PORT_6, 2); 

MeGyro gyro(0, 0x69); // onboard

MeRGBLed rgbled_0(0, 12); // 1-12 LED's | 0=all | onboard

void setup() {

  Serial.begin(9600);
  Serial.println("LOL");

  mysmartservo.begin(115200);
  mysmartservo.assignDevIdRequest();
  
  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);

  pinMode(dirPin2, OUTPUT);
  pinMode(stpPin2, OUTPUT);
  rgbled_0.setpin(44);
  int distGes = 0;
  gyro.begin();

  greifer.run(-50); //positive = close
  delay(3000);
  greifer.run(0);

  mysmartservo.move(1,-40,30);
  
  setColors(0,0,0);
  
}

void loop() {
  Serial.println("Let's start.");
  search_egg();
  Serial.println("Found an egg!");
  drive_to_egg();
  Serial.println("Yay here it is!");
  grabb_egg();
  Serial.println("GO GO GO GO");
  //drive_back();
}

void search_egg(){
  if(FIRST_RUN){
    TurnTo(-90);
    FIRST_RUN = false;
  }
  bool found_egg = false;
  int zAngle = gyro.getAngle(3);
  
  digitalWrite(dirPin,1);
  digitalWrite(dirPin2,1);
  while(!found_egg && zAngle<=90) {
    Serial.println(zAngle);
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800); 

    gyro.update();
    zAngle = gyro.getAngle(3);
    
    if(getUS() < MIN_DISTANCE)
    {
      found_egg=true;
      long start_time = millis();
      while(getUS() < MIN_DISTANCE){
        digitalWrite(stpPin, HIGH);
        digitalWrite(stpPin2, HIGH);
        delayMicroseconds(800);
        digitalWrite(stpPin, LOW);
        digitalWrite(stpPin2, LOW);
        delayMicroseconds(800); 
      }
      center();
    }
  }
  //else continue
}

void drive_to_egg(){
  
  int sensorState = lineFinder.readSensors();
  digitalWrite(dirPin,1);
  digitalWrite(dirPin2,0);
  delay(50);
  int i = 0;
  while(!limit_1.touched() && !limit_2.touched() && sensorState != S1_IN_S2_IN){
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800);
    int sensorState = lineFinder.readSensors();
    i++;
  }
  steps = i;
  if(sensorState == S1_IN_S2_IN){
    Serial.println("go back");
    setColors(255,0,0);
    _loop();
  }
  moveStraight(0, 200);
  delay(100);
  center();
  delay(100);
}

void grabb_egg(){
  //open grabber
  greifer.run(-30);
  delay(5000);
  greifer.run(0);
  //lower arm
  Serial.println("GO MOVE!");
  mysmartservo.move(1,-145,30);
  Serial.println("GO MOVE! END");
  delay(1000);
  //grab
  greifer.run(30);
  delay(5000);
  greifer.run(0);
  delay(1000);
  //rise arm
  Serial.println("GO MOVE REV!");
  mysmartservo.move(1,145,30);
  Serial.println("GO MOVE REV! END");
  delay(1000);
  //open grabber
  greifer.run(-30);
  delay(5000);
  greifer.run(0);
  //continue
  setColors(255,255,255);
}

drive_back(){
  digitalWrite(dirPin,0);
  digitalWrite(dirPin2,1);
  delay(50);
  i = i-200;
  while(i > 0){
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800);
    int sensorState = lineFinder.readSensors();
    i--;
  }
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
    //Serial.println(zAngle);
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
void center(){
  long end_time=millis();
  long duration = (end_time-start_time)/2;
  digitalWrite(dirPin,0);
  digitalWrite(dirPin2,0);
  delay(50);
  long second_time = millis();
  while( (millis()-second_time) < duration ){
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800); 
  }
}
double getUS()
{
  //Serial.print("US ");
  //Serial.println(ultraSensor.distanceCm());
  return ultraSensor.distanceCm();
}

void _loop(){
  while(true){
    
  }
}

