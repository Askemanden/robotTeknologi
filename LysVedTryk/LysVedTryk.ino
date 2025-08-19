const int ButtonIn = 4;

const int LEDPin = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(ButtonIn, INPUT);
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(ButtonIn) == HIGH){
    digitalWrite(LEDPin, HIGH);
  } else{
    digitalWrite(LEDPin, LOW);
  }
  delay(50);
}