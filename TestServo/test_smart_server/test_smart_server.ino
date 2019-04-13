#include <Arduino.h>
#include <Wire.h>
#include <MeAuriga.h>
#include "Wire.h"

MeSmartServo mysmartservo(PORT5);

void setup(){
   
    mysmartservo.begin(115200);
    mysmartservo.assignDevIdRequest();
    delay(50);

        mysmartservo.moveTo(1,90,30);
        delay(1000);
        mysmartservo.setInitAngle(1);
         delay(1000);
        mysmartservo.moveTo(1,-90,30);
         delay(1000);
        mysmartservo.setInitAngle(1);
         delay(1000);    
}
void loop(){

  
}

