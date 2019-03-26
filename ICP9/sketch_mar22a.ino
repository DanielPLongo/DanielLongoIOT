int led = 8;
unsigned int light;
char val;


void setup()
{
pinMode(A0,INPUT); //Light Sensor
pinMode(led, OUTPUT);  // Digital pin 13 set as output Pin
Serial.begin(9600);
delay(1000);
}

void loop()
{
  while (Serial.available() > 0)
  {
  val = Serial.read();
  Serial.println(val);
  }

  if( val == '1') // Forward
    {
     
      digitalWrite(led, HIGH);
    }
  else if(val == '2') // Backward
    {
 
      digitalWrite(led, LOW);
    }
  
   //digitalWrite(led, HIGH);
   light=analogRead(A0);
   Serial.write(light);
   Serial.println(light);
   delay(50);
}

// End of program
