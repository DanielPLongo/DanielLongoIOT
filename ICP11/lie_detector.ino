  int r5;
  int r4;
  int r3;
  int r2;
  int r1;
  int r2avg = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(3, HIGH);
  delay(100);
  digitalWrite(4, HIGH);
  delay(100);
}

void loop()
{
  r5=r4;
  r4=r3;
  r3=r2;
  r2=r1;
  r1=analogRead(A0);
  delay(200);
  int ravg = (r1+r2+r3+r4+r5)/5;
  r2avg = (7/8*(r2avg))+(1/8*(ravg));
  Serial.println(ravg);
  Serial.println(r2avg);
  if (ravg > 650) {
    digitalWrite(3,LOW);
    digitalWrite(4, HIGH);
  }
  else {
    digitalWrite(4,LOW);
    digitalWrite(3, HIGH);
  }
}
