const int LEDPin1 = 10;

unsigned long lastTime1 = 0;

unsigned long currentTime1 = 0;

bool LEDOn1 = false;

const int LEDPin2 = 12;

unsigned long lastTime2 = 0;

unsigned long currentTime2 = 0;

bool LEDOn2 = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime1 = millis();
  if(currentTime1 - lastTime1 >= 250){
    if(LEDOn1){
      digitalWrite(LEDPin1, LOW);
      LEDOn1 = false;
    }else{
      digitalWrite(LEDPin1, HIGH);
      LEDOn1 = true;
    }
    lastTime1 = currentTime1;
  }
    // put your main code here, to run repeatedly:
  currentTime2 = millis();
  if(currentTime2 - lastTime2 >= 1000){
    if(LEDOn1){
      digitalWrite(LEDPin2, LOW);
      LEDOn2 = false;
    }else{
      digitalWrite(LEDPin2, HIGH);
      LEDOn2 = true;
    }
    lastTime2 = currentTime2;
  }
  delay(50);
}