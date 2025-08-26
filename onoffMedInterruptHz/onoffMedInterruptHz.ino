#include <TimerOne.h>
const uint8_t ledPin1 = 10;
const uint8_t ledPin2 = 11;
int ledState1 = LOW;
int ledState2 = LOW;
int iterations = 1;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Timer1.initialize(250000); //The led will blink in a half second time interval
  Timer1.attachInterrupt(blinkLed);
}

void loop() {
  delay(5000);
}

void blinkLed(){
  iterations += 1;
  if(iterations > 4){
    iterations = 1;
    ledState2 = !ledState2;
  }
  ledState1 = !ledState1;
  digitalWrite(ledPin1, ledState1);
  digitalWrite(ledPin2, ledState2);
}

