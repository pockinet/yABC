#include <Streaming.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

long startingTime;
int buzzerPin = 10;
long isSeconds; 
int isMinutes = 0;
int targetTime;
int cyclestart = 0;
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
int targetMin = 0;
int targetSec = 0;
int timeEncoderPinALast = LOW;
int y = LOW;
int timerStart = 0;
int timeEncoderButtonPin = 8;
int tempEncoderButtonPin = 9;

void setup() {
  sensors.begin();
  pinMode (tempEncoderPinA, INPUT);
  pinMode (tempEncoderPinB, INPUT);
  Serial.begin (9600);
  startingTime = millis();
}


void loop() {
  killCycle();
  start();
  readSensors();
  readTempEncoder();
  readTimeEncoder();
  timecalc();
  timeCounter();
  heater();
  screen();
}

////////////////////////////////////////////////////////////////////////////

void screen(){
  Serial<<"Temp.: "<<sensors.getTempCByIndex(0)<<" °C"<<"/"<<targetTemp<<" °C"<<endl;
  Serial<<"Time: "<<isMinutes<<":"<<isSeconds<<"/"<<targetMin<<":"<<targetSec<<endl;
}

void readSensors(){
  sensors.requestTemperatures();
  sensors.getTempCByIndex(0);
}

void readTempEncoder(){
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

void readTimeEncoder(){
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
  if (sensors.getTempCByIndex(0) < targetTemp + overshoot && cyclestart == 1){
    digitalWrite(heaterPin, HIGH);
    timerStart = 1;
  }
  else {
    digitalWrite(heaterPin, LOW);
  }
}

void start(){
  if (digitalRead(tempEncoderButtonPin) == HIGH && digitalRead(tempEncoderButtonPin) == LOW){
    cyclestart = 1;
  }
}

void kill(){
  if (digitalRead(timeEncoderButtonPin) == HIGH){
    cyclestart = 0;
  }
}

void timeCounter(){
  if (timerStart == 1){
    isSeconds = millis() - startingTime;
    isSeconds = isSeconds / 1000;
    if (isSeconds == 60){
      isMinutes++;
      startingTime = millis();
    }
  }
}

void timecalc(){
  targetSec = targetTime;
  if (targetTime == 60){
    targetMin++;
    targetTime = 0;
  }
  if (targetTime < 0){
    targetMin--;
    targetTime = 59;
  }
  if (targetMin < 0){
    targetMin = 0;
  }
}

void killCycle(){
  if (targetMin == isMinutes && targetSec == isSeconds){
    cyclestart = 0;
    digitalWrite (buzzerPin, HIGH);
  } else {
      digitalWrite (buzzerPin, LOW);
  }
  if (digitalRead(timeEncoderButtonPin) == HIGH && digitalRead(tempEncoderButtonPin) == HIGH){
    cyclestart = 0;
  }
}

