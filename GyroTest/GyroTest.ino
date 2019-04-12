#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeAuriga.h>

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
MeGyro gyro_0(0, 0x69);



void setup(){
    Serial.begin(9600);
    gyro_0.begin();
}

void loop(){

    Serial.print("Z: ");
    Serial.println(gyro_0.getAngle(3));
    
    _loop();
}


void _loop(){
    gyro_0.update();
    
}

