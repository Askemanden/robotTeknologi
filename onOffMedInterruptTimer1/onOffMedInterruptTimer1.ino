#include <TimerOne.h>
const uint8_t ledPin = 4;
int ledState = LOW;
int iterations = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Timer1.initialize(500000); //The led will blink in a half second time interval
  Timer1.attachInterrupt(blinkLed);
}

void loop() {
  delay(2000);
}

void blinkLed(){
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
}

