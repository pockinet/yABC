#include <Streaming.h>    //Streaming library
#include <OneWire.h>    //OneWire library
#include <DallasTemperature.h>    //temperature-sensor library

#define ONE_WIRE_BUS 2    //temperature input is on pin 2
OneWire oneWire(ONE_WIRE_BUS);    //creates oneWire instance
DallasTemperature sensors(&oneWire);    //creates sensors instance

long startingTime;    //needed for timecounter
long isSeconds;     //actual seconds
int isMinutes = 0;    //actual minutes
int targetMin = 0;    //target minutes
int targetSec = 0;    //target seconds

int timerStart = 0;   //variable who starts the timer if "1"
int cyclestart = 0;   //if "1" cycle starts, if "0" cycle stops

int buzzerPin = 3;   //pin for the buzzer
int heaterPin = 4;    //pin for the heater ssr
int tempEncoderPinA = 5;    //pin A for the temperature encoder
int tempEncoderPinB = 6;    //pin B for the temperature encoder
int timeEncoderPinA = 7;    //pin A for the time encoder
int timeEncoderPinB = 8;    //pin B for the time encoder
int timeEncoderButtonPin = 9;   //pin for the button on the time encoder
int tempEncoderButtonPin = 10;   //pin for the button on the temperature encoder

int targetTemp = 50;    //initial target temperature

int tempEncoderPinALast = LOW;    //last state of the temperature encoder pin A
int timeEncoderPinALast = LOW;    //last state of the time encoder pin B
int x = LOW;    //needed variable for calculating temperature encoder
int y = LOW;    //needed variable for calculating time encoder

void setup() {
  sensors.begin();
  Serial.begin (9600);
  startingTime = millis();
}

void loop() {
  
//kills the cycle if target time is actual timeor both encoder pins are pushed and starts the buzzer for 5 seconds if cycle killed:
  if (targetMin == isMinutes && targetSec == isSeconds){
    cyclestart = 0;
    digitalWrite (buzzerPin, HIGH);
    delay(5000);
    digitalWrite (buzzerPin, LOW);
  } else {
      digitalWrite (buzzerPin, LOW);
  }
  if (digitalRead(timeEncoderButtonPin) == HIGH && digitalRead(tempEncoderButtonPin) == HIGH){
    cyclestart = 0;
    timerStart = 0;
    digitalWrite (buzzerPin, HIGH);
    delay(5000);
    digitalWrite (buzzerPin, LOW);
  }
  
//starts the cycle if the temperature encoder is pushed and the time encoder isn't:
  if (digitalRead(tempEncoderButtonPin) == HIGH && digitalRead(timeEncoderButtonPin) == LOW){
    cyclestart = 1;
  }
  
//reads sensors:
  sensors.requestTemperatures();
  sensors.getTempCByIndex(0);
  
//temperatur encoder reading:
  x = digitalRead(tempEncoderPinA);
  if ((tempEncoderPinALast == LOW) && (x == HIGH)) {
    if (digitalRead(tempEncoderPinB) == LOW) {
      targetTemp--;
    } else {
      targetTemp++;
    }
  }
  tempEncoderPinALast = x;
  
//time encoder reading:
  y = digitalRead(timeEncoderPinA);
  if ((timeEncoderPinALast == LOW) && (y == HIGH)) {
    if (digitalRead(timeEncoderPinB) == LOW) {
      targetMin--;
    } else {
      targetMin++;
    }
  }
  timeEncoderPinALast = y;

//calculates minutes from target time and prevents negative values:
  if (targetMin < 0){
    targetMin = 0;
  }

//calculates actual minutes and seconds:  
  if (timerStart == 1){
    isSeconds = millis() - startingTime;
    isSeconds = isSeconds / 1000;
    if (isSeconds == 60){
      isMinutes++;
      startingTime = millis();
    }
  }

//starts heater and timer if cycle started && actual temperature is lower then the target temperature:
if (sensors.getTempCByIndex(0) < targetTemp && cyclestart == 1){
    digitalWrite(heaterPin, HIGH);
    timerStart = 1;
  }
  else {
    digitalWrite(heaterPin, LOW);
  }

//prints actaul temp, target temp, passed time and target time to the serial monitor:
  Serial<<"Temp.: "<<sensors.getTempCByIndex(0)<<" °C"<<"/"<<targetTemp<<" °C"<<endl;
  Serial<<"Time: "<<isMinutes<<":"<<isSeconds<<"/"<<targetMin<<":"<<targetSec<<endl;
}
