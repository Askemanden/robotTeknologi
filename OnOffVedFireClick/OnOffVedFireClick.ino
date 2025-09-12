int counter = 0;
const int ButtonIn = 4;
bool lastRead = false;
bool currentRead = false;
const int LEDPin = 10;
bool LEDOn = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPin, OUTPUT);
  pinMode(ButtonIn, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentRead = digitalRead(ButtonIn);
  if(currentRead == HIGH && lastRead != HIGH){
    lastRead = HIGH;
    counter += 1;
  } else if(currentRead == LOW && lastRead != LOW){
    lastRead = LOW;
  }

  if(counter >=4){
    counter = 0;
    LEDOn = !LEDOn;
  }

  digitalWrite(LEDPin, LEDOn);

  delay(50);
}