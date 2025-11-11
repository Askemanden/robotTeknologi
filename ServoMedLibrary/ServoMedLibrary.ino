#include <Servo.h>

Servo Servo1; 

const uint8_t ServoPin = 3;

const uint8_t KnapPin = 7;

const uint8_t POTENTIOMETER = A1;

int ServoPosition;

void setup() {
  
  pinMode(ServoPin, OUTPUT);

  pinMode(KnapPin, INPUT);

  Servo1.attach(ServoPin);

  Serial.begin(9600);

  while(!digitalRead(KnapPin)) delay(50);

}


void loop() {

  ServoPosition = map(analogRead(POTENTIOMETER), 0, 1023, 0, 180);
  Servo1.write(ServoPosition);
  delay(15);
}
