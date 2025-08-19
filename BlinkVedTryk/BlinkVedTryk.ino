const int ButtonIn = 4;

const int LEDPin = 10;

unsigned long lastTime = 0;

unsigned long currentTime = 0;

bool LEDOn = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(ButtonIn, INPUT);
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  if(digitalRead(ButtonIn) == HIGH){
    if(currentTime - lastTime >= 250){
      if(LEDOn){
        digitalWrite(LEDPin, LOW);
        LEDOn = false;
      }else{
        digitalWrite(LEDPin, HIGH);
        LEDOn = true;
      }
      lastTime = currentTime;
    }
  } else{
    digitalWrite(LEDPin, LOW);
  }
  delay(50);
}