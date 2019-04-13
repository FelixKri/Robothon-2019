#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>

MeGyro gyro_0(0, 0x69);

MeRGBLed rgbled_0(0, 12);

#define TURN_TO_THRESHOLD 2

int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2

int dirPin2 = mePort[PORT_2].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin2 = mePort[PORT_2].s2;//the Step pin connect to Base Board PORT1 SLOT2

void setup(){
  Serial.begin(9600);
    gyro_0.begin();
    rgbled_0.setpin(44);
    rgbled_0.setColor(0,0,0,0);
    rgbled_0.show();

      pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);

  pinMode(dirPin2, OUTPUT);
  pinMode(stpPin2, OUTPUT);
}

void loop(){
  TurnTo(90);
  delay(1000);
  TurnTo(0);
  delay(1000);
}

void TurnTo(int angle)
{
int zAngle = gyro_0.getAngle(3);
    digitalWrite(dirPin,zAngle<angle);
  digitalWrite(dirPin2,zAngle<angle);
  delay(50);
  while(abs (zAngle-angle)> TURN_TO_THRESHOLD)
  {
    Serial.println(zAngle);
    digitalWrite(stpPin, HIGH);
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800); 

    gyro_0.update();
    zAngle = gyro_0.getAngle(3);
  }
}


void turn (boolean dir, int steps) //0=right, 1=left
{
  digitalWrite(dirPin,dir);
  digitalWrite(dirPin2,dir);
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
