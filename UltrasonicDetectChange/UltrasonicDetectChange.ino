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
  
  for(int i=0; i < 100; i++){
    range = ultrasonic.MeasureInCentimeters();
    Serial.println(range, DEC);
    if (range != 0 && range <= 400)
    {
      wall[count] = range;
      Serial.print(count);
      Serial.print("range: ");
      Serial.println(range, DEC);
      count++;
    }
    delay(60);
  };
  
  //Serial.println(count);
  
  for(int i=0; i < count; i++){
    sum += wall[i];
  };
  
  //Serial.println(sum);

  wallDist = (sum/count) - 15;
  if (wallDist < 5)
  {
    wallDist = 400;
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









