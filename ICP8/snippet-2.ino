String data="Hello From Arduino!";

void setup() {
// put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {
// put your main code here, to run repeatedly:
Serial.println(data);//data that is being Sent
delay(200);
}
