#include <Servo.h>

Servo myServo;
const uint8_t POT = A1;

void setup()
{
  pinMode(POT, INPUT);
  myServo.attach(3);
  Serial.begin(9600);
}

void loop() {
  int reading = analogRead(POT);
  int speed = int((float(reading)/1024)*800);
  Serial.println(speed);

  myServo.writeMicroseconds(1702 + speed);
}