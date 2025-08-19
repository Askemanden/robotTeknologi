const int LEDPins[] = {10,11,12,13};

int maxIndex = 3;

int LEDPin = LEDPins[0];

int index = 0;

const int ButtonIn = 4;

unsigned long lastTime = 0;

unsigned long currentTime = 0;

bool LEDOn = false;

const int blinkDelay = 250;

void setup() {
  // put your setup code here, to run once:
  pinMode(ButtonIn, INPUT);
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  if(digitalRead(ButtonIn) == HIGH){
    if(currentTime - lastTime >= blinkDelay){
      if(LEDOn){
        digitalWrite(LEDPin, LOW);
        LEDOn = false;
        index += 1;
        if(index > maxIndex){
          index = 0;
        }
        LEDPin = LEDPins[index];
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

