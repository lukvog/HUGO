#include "Ultrasonic.h"

Ultrasonic ultrasonic(21);


int changed;
int wallDist;
int range;



void setup()
{
  Serial.begin(9600);

  int wall[100];
  int count = 0;
  int sum = 0;

  //calibration for Walls

  for(int i=0; i <= 100; i++){
    range = ultrasonic.MeasureInCentimeters();
    if (range != 0 && range <= 450)
    {
      wall[i] = range;
      count++;
      Serial.print(count);
      Serial.print("range: ");
      Serial.println(range, DEC);
    }
    delay(50);
  };
  for(int i=0; i <= count; i++){
    sum = sum + wall[i];
  };

  wallDist = (sum/count) - 5;
  if (wallDist < 5)
  {
    wallDist = 450;
  }

}

void loop()
{

  range = ultrasonic.MeasureInCentimeters();

  if (range != 0 && range < wallDist)
  {
    changed = range;
  }

  Serial.print("wallDist: ");
  Serial.println(wallDist, DEC);
  Serial.print("changed: ");
  Serial.println(changed, DEC);
  /*
  Serial.println("The distance to obstacles in front is: ");
   Serial.print(range);//0~400cm
   Serial.println(" cm");
   */
  delay(50);
}









