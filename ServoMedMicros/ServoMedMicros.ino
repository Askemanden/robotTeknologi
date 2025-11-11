const uint8_t ServoPin = 3;
const uint8_t KnapPin = 7;
int PulseWidth;

const uint8_t POTENTIOMETER = A1;

unsigned long TimeNow, TimeBefore;

const int TIME_DELAY = 20000;

void setup() {
  
  pinMode(ServoPin, OUTPUT);

  pinMode(KnapPin, INPUT);

  TimeBefore = micros();

  Serial.begin(9600);

  while(!digitalRead(KnapPin)) delay(50);

}


void loop() {

  TimeNow = micros();

  if(TimeNow - TimeBefore >= TIME_DELAY){

    TimeBefore = TimeNow;
    PulseWidth = map(analogRead(POTENTIOMETER),0,1023,600, 2400);
    digitalWrite(ServoPin, HIGH);
    delayMicroseconds(PulseWidth);
    digitalWrite(ServoPin, LOW);
 
  }
       
}
