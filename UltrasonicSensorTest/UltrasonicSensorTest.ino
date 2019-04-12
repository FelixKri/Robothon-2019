#include "MeAuriga.h"

MeUltrasonicSensor ultraSensor(PORT_7); /* Ultrasonic module can ONLY be connected to port 3, 4, 6, 7, 8 of base shield. */

double searchingLength = 75.0;
double previousMeasured = searchingLength;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println(scan(previousMeasured));
}

bool scan(double previous){

    double measured = ultraSensor.distanceCm();

    double variance = abs(measured - previous);
    Serial.print("absolute Variance: ");
    Serial.println(variance);

    previousMeasured = measured;

    if(variance > 5.0 && measured < searchingLength){
      return true;
    }
    return false;

}

