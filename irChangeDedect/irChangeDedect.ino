int changed;
int wallDist;
float irVal;
int irPin = 21;

//smoothing
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int indx = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


void setup()
{
  Serial.begin(9600);

  int wall[100];
  int count = 0;
  int sum = 0;

  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0; 

  //calibration for Walls

  for(int i=0; i < 100; i++){
    irVal = analogRead(irPin);
    // Linearize Sharp GP2YOA1YK
    float irDist = 11.0e8 * pow(irVal, -2.438);
    //float irDist = 1000/irVal;

    if (irDist >= 100 && irDist <= 550)  
    {

      wall[count] = irDist;
      Serial.print(count);
      Serial.print("range: ");
      Serial.println(irDist, DEC);
      count++;
    }
    delay(50);
  };

  //Serial.println(count);

  for(int i=0; i < count; i++){
    sum += wall[i];
  };

  //Serial.println(sum);

  wallDist = (sum/count) - 30;
  if (wallDist < 50)
  {
    wallDist = 550;
  }


}

void loop()
{

  irVal = analogRead(irPin);
  // Linearize Sharp GP2YOA1YK
  float irDist = 11.0e8 * pow(irVal, -2.438);
  //Serial.println(irVal, DEC);

  //float irDist = 1000/irVal;

  // subtract the last reading:
  total= total - readings[indx];        
  // read from the sensor:  
  readings[indx] = irDist;
  // add the reading to the total:
  total= total + readings[indx];      
  // advance to the next position in the array:  
  indx = indx + 1;                    

  // if we're at the end of the array...
  if (indx >= numReadings)              
    // ...wrap around to the beginning:
    indx = 0;                          

  // calculate the average:
  average = total / numReadings; 

  Serial.println(irDist, DEC);

    if (average >= 50 && average <= wallDist)
    {
      changed = average;
    }

  Serial.print("wallDist: ");
  Serial.println(wallDist, DEC);
  Serial.print("changed: ");
  Serial.println(changed, DEC);
  delay(50);
}













