const int LEDPins[] = {10,11,12,13};

int maxIndex = 3;

int LEDPin = LEDPins[0];

int index = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDPin, HIGH);
  Serial.println("VI TÆNDER, YIPPEEE");
//  delay(500);
  delay(2000);
  digitalWrite(LEDPin, LOW);
  Serial.println("vi slukker, øvvvvv :((");
  delay(2000);
//  delay(500);
  index += 1;
  if(index > maxIndex){
    index = 0;
  }
  LEDPin = LEDPins[index];
}