const uint8_t ledPin = 10;
const uint8_t interruptPin = 2;  // input pin that the interruption will be attached to
volatile uint8_t state = LOW;  // variable that will be updated in the ISR

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);
}

void loop() {
  delay(5000);
}

void blink() {
  state = !state;
  digitalWrite(ledPin, state);
}