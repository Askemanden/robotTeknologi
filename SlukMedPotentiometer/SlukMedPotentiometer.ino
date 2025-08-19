const int POTENTIOMETER = A5;

const int LEDPins[] = {10,11,12,13};

int maxIndex = 3;

int LEDPin = LEDPins[0];

int index = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPin, OUTPUT);
  pinMode(POTENTIOMETER, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  const int lys = analogRead(POTENTIOMETER);
  if(lys>700){
    digitalWrite(LEDPin, HIGH);
  } else { 
    digitalWrite(LEDPin, LOW);
  }
  Serial.println(lys);
  delay(50);
}
