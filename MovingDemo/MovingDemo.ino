#include "MeAuriga.h"

int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2

int dirPin2 = mePort[PORT_2].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin2 = mePort[PORT_2].s2;//the Step pin connect to Base Board PORT1 SLOT2

void setup()
{
  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);

  pinMode(dirPin2, OUTPUT);
  pinMode(stpPin2, OUTPUT);
}

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
void loop()
{
  moveStraight(1,600);//run 200 step
  delay(1000);
  turn(1,600);
  delay(1000);
  turn(0,600);
  delay(1000);
  moveStraight(0,600);//run 200 step
  delay(1000);
}
