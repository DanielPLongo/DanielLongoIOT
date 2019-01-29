int switchState = 0;
void setup() {
  //the following code initializes the pins to either input or output
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, INPUT);
}

void loop(){
  switchState = digitalRead(2);
  if(switchState == LOW){
  // the button is not pressed and pin 2 is LOW

  digitalWrite(3, HIGH); //green LED ON
  digitalWrite(4, LOW); //yellow LED OFF
  digitalWrite(5, LOW); //red LED OFF
  }

  else{
    //the button is pressed and pin 2 is HIGH
      digitalWrite(3, LOW); //green LED OFF
      digitalWrite(4, HIGH); //yellow LED ON
      digitalWrite(5, LOW); //red LED OFF
      delay(250); //this is expressed in ms, so .25sec
      
      digitalWrite(3, LOW); //green LED OFF
      digitalWrite(4, LOW); //yellow LED OFF
      digitalWrite(5, HIGH); //red LED ON
      delay(250);
    }
  }

}
