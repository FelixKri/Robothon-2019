#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// ---------------------- Global Var's ---------------------
#define MIN_DISTANCE 80
#define TURN_TO_THRESHOLD 2
bool FIRST_RUN = true;
int steps;
bool found_egg = false;
// ---------------------- Motors/Sensors -------------------
int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2

int dirPin2 = mePort[PORT_2].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin2 = mePort[PORT_2].s2;//the Step pin connect to Base Board PORT1 SLOT2

MeDCMotor greifer(PORT_3);

MeSmartServo mysmartservo(PORT5);

MeUltrasonicSensor ultraSensor(PORT_7);

MeLineFollower lineFinder(PORT_8);

MeCompass compas(PORT_10);

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
  compas.begin();

  greifer.run(-50); //positive = close
  delay(3000);
  greifer.run(0);

  mysmartservo.move(1,-40,30);
  
  setColors(0,0,0);
  
}

void loop() {
  Serial.println("Let's start.");

  search_egg();
  if(found_egg){
    Serial.println("Found an egg!");
    drive_to_egg();
  }if(found_egg){
    Serial.println("Yay here it is!");
    grabb_egg();
  }if(found_egg){
    Serial.println("GO GO GO GO");
    drive_back();
  }
  toNext();
}

void search_egg(){
  if(FIRST_RUN){
    TurnTo(-90);
    FIRST_RUN = false;
  }
  int zAngle = gyro.getAngle(3);
  
  digitalWrite(dirPin,1);
  digitalWrite(dirPin2,1);
  
  int i = 0;
  
  while(!found_egg) {
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800); 

    if(i == 9){
      found_egg = getUSbool();
      i = 0;
    }
    i++;
  
    //gyro.update();
    //zAngle = gyro.getAngle(3);
  }
  if(found_egg)
  {
    setColors(0,255,0);
    double last_distance = getUS();
    digitalWrite(dirPin,1);
    digitalWrite(dirPin2,1);
    delay(50);
    /*bool dummy = true;
    while(getUS() < MIN_DISTANCE && dummy){
          digitalWrite(stpPin, HIGH);
          digitalWrite(stpPin2, HIGH);
          delayMicroseconds(800);
          digitalWrite(stpPin, LOW);
          digitalWrite(stpPin2, LOW);
          delayMicroseconds(800); 
          if(last_distance < getUS())
             dummy = false;
    }*/
    long start_time = millis();
    while(getUS() < MIN_DISTANCE){
          digitalWrite(stpPin, HIGH);
          digitalWrite(stpPin2, HIGH);
          delayMicroseconds(800);
          digitalWrite(stpPin, LOW);
          digitalWrite(stpPin2, LOW);
          delayMicroseconds(800); 
    }
    long end_time=millis();
    long duration = (end_time-(start_time))/2;
    digitalWrite(dirPin,0);
    digitalWrite(dirPin2,0);
    delay(50);
    long second_time = millis();
    double distance = 100000.0;
    while( ((millis()-second_time) < duration)){
      if(distance > getUS()){
        setColors(0,150,0);
      }
      distance = getUS();
      digitalWrite(stpPin, HIGH);
      digitalWrite(stpPin2, HIGH);
      delayMicroseconds(800);
      digitalWrite(stpPin, LOW);
      digitalWrite(stpPin2, LOW);
      delayMicroseconds(800); 
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
    found_egg = false;
  }
  moveStraight(0, 100);
  delay(100);
}

void grabb_egg(){
  //open grabber
  greifer.run(-30);
  delay(5000);
  greifer.run(0);
  //lower arm
  mysmartservo.move(1,-145,30);
  delay(1000);
  //grab
  greifer.run(30);
  delay(5000);
  greifer.run(0);
  delay(1000);
  //rise arm
  mysmartservo.move(1,145,30);
  delay(1000);
  //open grabber
  greifer.run(-30);
  delay(5000);
  greifer.run(0);
  //continue
  setColors(255,255,255);
}

void drive_back(){
  digitalWrite(dirPin,0);
  digitalWrite(dirPin2,1);
  delay(50);
  steps = steps/2;
  while(steps > 0 && lineFinder.readSensors() != S1_IN_S2_IN){
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800);
    steps--;
  }
  toNext();
}

void toNext(){
  delay(100);
  TurnTo(0);
  moveStraight(1, 2000);
  FIRST_RUN = true;
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
    if(lineFinder.readSensors() == S1_IN_S2_IN){
      toNext();
    }
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
void center(long start_time){
  digitalWrite(dirPin,1);
  digitalWrite(dirPin2,1);
  delay(50);
  while(getUS() < MIN_DISTANCE){
        digitalWrite(stpPin, HIGH);
        digitalWrite(stpPin2, HIGH);
        delayMicroseconds(800);
        digitalWrite(stpPin, LOW);
        digitalWrite(stpPin2, LOW);
        delayMicroseconds(800); 
   }
  long end_time=millis();
  long duration = (end_time-(start_time+50))/2;
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
  return ultraSensor.distanceCm();
}
bool getUSbool(){
  if(ultraSensor.distanceCm()<MIN_DISTANCE)
    return true;
  return false;
}

void _loop(){
  while(true){
    
  }
}

