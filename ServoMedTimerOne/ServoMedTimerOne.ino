#include <TimerOne.h>
  
const uint8_t ServoPin = 3;
const uint8_t KnapPin = 7;
int PulseWidth;

const uint8_t POTENTIOMETER = A1;

unsigned long TimeNow, TimeBefore;

const int TIME_DELAY = 20000;

  

void setup() {
  
  pinMode(ServoPin, OUTPUT);

  pinMode(KnapPin, INPUT);

  Timer1.initialize(TIME_DELAY);
  Timer1.attachInterrupt(Pulse);

  TimeBefore = millis();

  Serial.begin(9600);

  while(!digitalRead(KnapPin)) delay(50);

}


void Pulse(){
  PulseWidth = map(analogRead(POTENTIOMETER),0,1023,600, 2400);
  digitalWrite(ServoPin,HIGH);
  delayMicroseconds(PulseWidth);
  digitalWrite(ServoPin,LOW);
}


void loop() {
  static uint64_t jacob = 2;
  jacob *= 3021;
  Serial.println(jacob*2.3+47-1.2/jacob);
  jacob /= jacob*2+1;
  /*medtaget for at illustrere at kode kan køre i parallel*/
}
