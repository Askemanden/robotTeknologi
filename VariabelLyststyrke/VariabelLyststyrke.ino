const int POTENTIOMETER = A5;

int LEDPin = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPin, OUTPUT);
  pinMode(POTENTIOMETER, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  const int lys = analogRead(POTENTIOMETER);
  analogWrite(LEDPin, lys/4);
  Serial.println(lys);
  delay(50);
}
