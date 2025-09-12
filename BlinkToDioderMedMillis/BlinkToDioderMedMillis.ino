const int LEDPin1 = 10;

unsigned long lastTime1 = 0;

unsigned long currentTime = 0;

bool LEDOn1 = false;

const int LEDPin2 = 12;

unsigned long lastTime2 = 0;

bool LEDOn2 = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  if(currentTime - lastTime1 >= 250){
    if(LEDOn1){
      digitalWrite(LEDPin1, LOW);
      LEDOn1 = false;
    }else{
      digitalWrite(LEDPin1, HIGH);
      LEDOn1 = true;
    }
    lastTime1 = currentTime;
  }

  if(currentTime - lastTime2 >= 1000){
    if(LEDOn2){
      digitalWrite(LEDPin2, LOW);
      LEDOn2 = false;
    }else{
      digitalWrite(LEDPin2, HIGH);
      LEDOn2 = true;
    }
    lastTime2 = currentTime;
  }
  delay(50);
}