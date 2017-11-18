#include <Streaming.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int startTime;
int overshoot = 2;
int heaterPin = 7;
int tempEncoderPinA = 3;
int tempEncoderPinB = 4;
int targetTemp = 20;
int tempEncoderPinALast = LOW;
int x = LOW;
int timeEncoderPinA = 5;
int timeEncoderPinB = 6;
int targetTime = 0;
int timeEncoderPinALast = LOW;
int y = LOW;
int timerStart = 0;
int previousMillis = 0;
int timeEncoderButtonPin = 8;

void setup() {
  sensors.begin();
  pinMode (tempEncoderPinA, INPUT);
  pinMode (tempEncoderPinB, INPUT);
  Serial.begin (9600);
  Serial<<"Welcome!"<<endl;
}


void loop() {
  unsigned long interval = minuten * 60 * 1000UL;
  sensors.requestTemperatures();
  sensors.getTempCByIndex(0)
  readTempEncoder();
  readTimeEncoder(); 
  homescreen();
  heater();
}

////////////////////////////////////////////////////////////////////////////

void homescreen(){
  Serial<<"Temp.: "<<sensors.getTempCByIndex(0)<<" °C"<<"/"<<targetTemp<<" °C"<<endl
}

void readTempEncoder() {
  x = digitalRead(tempEncoderPinA);
  if ((tempEncoderPinALast == LOW) && (x == HIGH)) {
    if (digitalRead(tempEncoderPinB) == LOW) {
      targetTemp--;
    } else {
      targetTemp++;
    }
  }
  tempEncoderPinALast = x;
}

void readTimeEncoder() {
  y = digitalRead(timeEncoderPinA);
  if ((timeEncoderPinALast == LOW) && (y == HIGH)) {
    if (digitalRead(timeEncoderPinB) == LOW) {
      targetTime--;
    } else {
      targetTime++;
    }
  }
  timeEncoderPinALast = y;
}

void heater(){
  if (sensors.getTempCByIndex(0) < tagrgetTemp + overshoot && start == 1){
    digitalWrite(heaterPin, HIGH);
    timerStart = 1;
  }
  else {
    digitalWrite(heaterPin, LOW);
  }
}

void timer(){
  if (timerstart = 1){
    previousMillis == millis();
  }
  if(millis() - previousMillis > targetTime){
  
  }  
}

void start(){
  if (digitalRead(tempEncoderButtonPin) == 1){
    start = 1;
  }
}

void kill(){
  if (digitalRead(timeEncoderButtonPin) == 1){
    start = 0;
  }
}


