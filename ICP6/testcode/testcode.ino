
#include <SoftwareSerial.h>
#define DEBUG true

#include <stdlib.h>
void setup() {
  // put your setup code here, to run once:
pinMode(5,OUTPUT); //pin for LED
}

void loop() 
{
  int t=0;
 while (t=0)
   {
  digitalWrite(5,HIGH);
  delay(1000);
  digitalWrite(5,LOW);
  delay(1000);
   }
}
