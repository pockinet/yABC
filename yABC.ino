#include <Streaming.h>    //Streaming library
#include <OneWire.h>    //OneWire library
#include <DallasTemperature.h>    //temperature-sensor library

#define ONE_WIRE_BUS 2    //temperature input is on pin 2
OneWire oneWire(ONE_WIRE_BUS);    //creates oneWire instance
DallasTemperature sensors(&oneWire);    //creates sensors instance

long startingTime;    //needed for timecounter
int buzzerPin = 10;   //pin for the buzzer
long isSeconds;     //actual seconds
int isMinutes = 0;    //actual minutes
int targetTime;   //target time
int cyclestart = 0;   //if "1" cycle starts, if "0" cycle stops
int overshoot = 1;    //number of °C for heater over- and undershoot
int heaterPin = 7;    //pin for the heater ssr
int tempEncoderPinA = 3;    //pin A for the temperature encoder
int tempEncoderPinB = 4;    //pin B for the temperature encoder
int targetTemp = 20;    //target temperature
int tempEncoderPinALast = LOW;    //last state of the temperature encoder pin A
int x = LOW;    //needed variable for calculating temperature encoder
int timeEncoderPinA = 5;    //pin A for the time encoder
int timeEncoderPinB = 6;    //pin B for the time encoder
int targetMin = 0;    //target minutes
int targetSec = 0;    //target seconds
int timeEncoderPinALast = LOW;    //last state of the time encoder pin B
int y = LOW;    //needed variable for calculating time encoder
int timerStart = 0;   //variable who starts the timer if "1"
int timeEncoderButtonPin = 8;   //pin for the button on the time encoder
int tempEncoderButtonPin = 9;   //pin for the button on the temperature encoder

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
  readSensor();
  readTempEncoder();
  readTimeEncoder();
  timecalc();
  timeCounter();
  heater();
  screen();
}

////////////////////////////////////////////////////////////////////////////

//sends actual temperature/target temperatur to serial monitor
//sends actual minutes:seconds/target minutes:seconds to serial monitor
void screen(){
  Serial<<"Temp.: "<<sensors.getTempCByIndex(0)<<" °C"<<"/"<<targetTemp<<" °C"<<endl;
  Serial<<"Time: "<<isMinutes<<":"<<isSeconds<<"/"<<targetMin<<":"<<targetSec<<endl;
}

//temperature sensor
void readSensor(){
  sensors.requestTemperatures();
  sensors.getTempCByIndex(0);
}

//temperature encoder
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

//time encoder
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

//if cycle started and the actual temperature is lower then the target temperatur plus overshoot the heater starts
//the timer starts
void heater(){
  if (sensors.getTempCByIndex(0) < targetTemp + overshoot && cyclestart == 1){
    digitalWrite(heaterPin, HIGH);
    timerStart = 1;
  }
  else {
    digitalWrite(heaterPin, LOW);
  }
}

//if the temperature encoder is pushed and the time encoder isn't then the cycle starts
void start(){
  if (digitalRead(tempEncoderButtonPin) == HIGH && digitalRead(timeEncoderButtonPin) == LOW){
    cyclestart = 1;
  }
}

//calculates actual minutes 
//calculates actual seconds
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

//calculates minutes from target time
//calculates seconds from target time
//prevents underflow for minutes and seconds
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

//kills the cycle if target time is actual time 
//kills the cycle if both encoder pins are pushed
//starts the buzzer for 5 seconds if cycle killed
void killCycle(){
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
}

