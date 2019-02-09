
#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial esp8266(9,10); 
#include <LiquidCrystal_PCF8574.h>
#include <stdlib.h>
#include <DHT.h>
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//******Temperature*******
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//******Barometer********
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C

//********Light***********
int light;

//********Dust***********
int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000; 
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

//********UV***********
float Vsig;


#define SSID "490RoProg1"     // "SSID-WiFiname" 
#define PASS "490RoPro"       // "password"
#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=CQ5PC2S1M5ZC34VB"; //change it with your api key like "GET /update?key=Your Api Key"


//Variables
float temp;
int hum;
String tempC;
int error;
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;
int fadeRate = 0;

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = true;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse 


void setup()
{
  lcd.setBacklight(255);
  lcd.begin(16, 2);
  lcd.print("circuitdigest.com");
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("Connecting...");
  Serial.begin(115200); //or use default 115200.
  esp8266.begin(115200);
  Serial.println("AT");
  esp8266.println("AT");
  delay(5000);
  if(esp8266.find("OK")){
    connectWiFi();
  }
 
  
  pinMode(A0,INPUT); //pin for Light sensor

  pinMode(8,INPUT); //pin for Dust sensor


}

void loop(){
  lcd.clear();
  lcd.setCursor(0,0);
 //Temp Sensor
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    lcd.print("Humidity: ");
    lcd.print(hum);
    lcd.print("%");
    lcd.setCursor(0, 1); // set the cursor to column 0, line 2
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.println(" *C");
    delay(2000); //Delay 2 sec.
    updatebeat('1', hum);
    updatebeat('2', temp);
    

//Barometer
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print(F("Temperature = "));
//    lcd.print(bmp.readTemperature());
//    lcd.println(" *C");
    
    lcd.print(F("Pressure = "));
    lcd.print(bmp.readPressure());
    lcd.println(" Pa");
    lcd.setCursor(0,1);
    lcd.print(F("Altitude = "));
    lcd.print(bmp.readAltitude(29.92)); // this should be adjusted to your local forcase
    lcd.println(" m");    
    lcd.println();
    updatebeat('3', bmp.readPressure());
    updatebeat('4', bmp.readAltitude(29.92));    
    delay(2000);

//Light Sensor
  lcd.clear();
  lcd.setCursor(0,0);
  light=analogRead(A0);
  lcd.println("Light = " + String(light));
  updatebeat('5', light);
  delay(2000);    


//Dust Sensor
  lcd.clear();
  lcd.setCursor(0,0);
  starttime = millis();
  delay(2000);
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; 
    lcd.print("CX = ");
    lcd.print(concentration);
    lcd.println(" pcs/0.01cf");
    lcd.println("\n");
    lowpulseoccupancy = 0;
    updatebeat('6', concentration);
    delay(2000); 
  }


//UV Sensor
  int sensorValue;
  long  sum=0;
  for(int i=0;i<1024;i++)
   {  
      sensorValue=analogRead(A1);
      sum=sensorValue+sum;
      delay(2);
   }   
 sum = sum >> 10;
 Vsig = sum*4980.0/1023.0; // Vsig is the value of voltage measured from the SIG pin of the Grove interface
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("UV Level: ");
 lcd.print(Vsig);
 lcd.print(" mV    --   ");
 
//if (Vsig < 50) {
//    Serial.print("UV Index: 0 "); Serial.println("   Exposure level - NONE (You're probably at home!) ");
// }
//if (Vsig > 50 && Vsig < 227) {
//    Serial.print("UV Index: 1 "); Serial.println("   Exposure level - LOW (You're probably at home!) ");
// }
//if (Vsig > 227 && Vsig < 318) {
//    Serial.print("UV Index: 2 "); Serial.println("   Exposure level - LOW (You can go outside and have fun!) ");
// }
//if (Vsig > 318 && Vsig < 408) {
//    Serial.print("UV Index: 3 "); Serial.println("   Exposure level - MODERATE (Sun starts to annoy you) ");
// }
//if (Vsig > 408 && Vsig < 503) {
//    Serial.print("UV Index: 4 "); Serial.println("   Exposure level - MODERATE (Sun starts to annoy you) ");
// }
//if (Vsig > 503 && Vsig < 606) {
//    Serial.print("UV Index: 5 "); Serial.println("   Exposure level - MODERATE (Sun starts to annoy you) ");
// }
//if (Vsig > 606 && Vsig < 696) {
//    Serial.print("UV Index: 6 "); Serial.println("   Exposure level - HIGH (Get out from the sunlight! get out now!) ");
// }
//if (Vsig > 696 && Vsig < 795) {
//    Serial.print("UV Index: 7 "); Serial.println("   Exposure level - HIGH (Get out from the sunlight! get out now!) ");
// }
//if (Vsig > 795 && Vsig < 881) {
//    Serial.print("UV Index: 8 "); Serial.println("   Exposure level - VERY HIGH (Get out from the sunlight! get out now!) ");
// }
//if (Vsig > 881 && Vsig < 976) {
//    Serial.print("UV Index: 9 "); Serial.println("   Exposure level - VERY HIGH (If you value your health, don't go outside, just stay at home!) ");
// }
//if (Vsig > 976 && Vsig < 1079) {
//    Serial.print("UV Index: 10 "); Serial.println("   Exposure level - VERY HIGH (If you value your health, don't go outside, just stay at home!) ");
// }
//if (Vsig > 1079 && Vsig < 1170) {
//    Serial.print("UV Index: 11 "); Serial.println("   Exposure level - EXTREME (If you value your health, don't go outside, just stay at home!) ");
// }
//if (Vsig > 1170) {
//    Serial.print("UV Index: 11+ "); Serial.println("   Exposure level - EXTREME (You will probably die in 3, 2, 1... Just JOKING, don't be scared...) - intensity of sunlight is really at maximum ");
// }
    updatebeat('7',Vsig);
    delay(2000);




  start: //label 
  error=0;
  delay(100);
  //updatebeat();
  //Resend if transmission is not completed 
  if (error==1){
    goto start; //go to label "start"
  }
  delay(100); 
}

void updatebeat(char field, float value){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(200);
  if(esp8266.find("Error")){
    return;
  }
  cmd = msg ;
  cmd += "&field";
  cmd += field;
  cmd += "=";
  cmd += value;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
 // if(esp8266.find(">")){
    Serial.print(cmd);
    esp8266.print(cmd);
 // }
  //else{
  // lcd.println("AT+CIPCLOSE");
   //esp8266.println("AT+CIPCLOSE");
   // //Resend...
   // error=1;
  //}
  delay(500);
}

boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(3000);
  if(esp8266.find("OK")){
    Serial.println("OK");
    return true;    
  }else{
    return false;
  }
}

void interruptSetup(){     
  TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER 
  OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED      
} 
