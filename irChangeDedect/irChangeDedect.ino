int changed;
int wallDist;
int irVal;
int irPin = 21;

void setup()
{
  Serial.begin(9600);
  
  int wall[100];
  int count = 0;
  int sum = 0;

  //calibration for Walls
  
  for(int i=0; i < 100; i++){
    irVal = analogRead(irPin);
    // Linearize Sharp GP2YOA1YK
    float irDist = 9.8122e8 * pow(irVal, -2.438);
    
    if (irDist >= 50 && irDist <= 550)
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

  wallDist = (sum/count) - 15;
  if (wallDist < 5)
  {
    wallDist = 400;
  }
  

}

void loop()
{

  irVal = analogRead(irPin);
  // Linearize Sharp GP2YOA1YK
  float irDist = 9.8122e8 * pow(irVal, -2.438);
  
  Serial.println(irDist, DEC);

  if (irDist >= 50 && irDist <= wallDist)
  {
    changed = irDist;
  }

  Serial.print("wallDist: ");
  Serial.println(wallDist, DEC);
  Serial.print("changed: ");
  Serial.println(changed, DEC);
  delay(50);
}









