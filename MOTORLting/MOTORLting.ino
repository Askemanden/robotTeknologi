const int states[4][4] = {
  { 0,  1,  0, -1},
  {-1,  0,  1,  0},
  { 0, -1,  0,  1},
  { 1,  0, -1,  0}
};

const uint8_t ENCODER_OUT_A = 3;
const uint8_t ENCODER_OUT_B = 2;
const uint8_t LED_PIN       = 4;
const uint8_t FULL_ROTATION = 64*30;

volatile bool led_state = LOW;
volatile int position = 0;

volatile uint8_t last_value;
volatile uint8_t current_value;

volatile unsigned long lastInterruptTime = 0; // Til debounce

void change() {
  unsigned long now = micros();
  if (now - lastInterruptTime < 1000) return;  // 1 ms debounce
  lastInterruptTime = now;

  int A_READ = digitalRead(ENCODER_OUT_A);
  int B_READ = digitalRead(ENCODER_OUT_B);

  last_value = current_value;

  // Kodér A/B-tilstand som tal 0–3
  if (A_READ == HIGH && B_READ == HIGH) {
    current_value = 0;
  } else if (A_READ == LOW && B_READ == HIGH) {
    current_value = 1;
  } else if (A_READ == LOW && B_READ == LOW) {
    current_value = 2;
  } else if (A_READ == HIGH && B_READ == LOW) {
    current_value = 3;
  }

  // Opdater position baseret på overgangstabel
  position += states[last_value][current_value];

  // Når vi har nået en fuld rotation – skift LED
  if (position >= FULL_ROTATION || position <= -FULL_ROTATION) {
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    Serial.print("LED skiftet! Ny tilstand: ");
    Serial.println(led_state ? "TÆNDT" : "SLUKKET");

    if (position > 0) position -= FULL_ROTATION;
    else position += FULL_ROTATION;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(ENCODER_OUT_A, INPUT_PULLUP);
  pinMode(ENCODER_OUT_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_OUT_A), change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_OUT_B), change, CHANGE);

  // Initiel aflæsning
  int A_READ = digitalRead(ENCODER_OUT_A);
  int B_READ = digi talRead(ENCODER_OUT_B);

  if (A_READ == HIGH && B_READ == HIGH) current_value = 0;
  else if (A_READ == LOW && B_READ == HIGH) current_value = 1;
  else if (A_READ == LOW && B_READ == LOW) current_value = 2;
  else if (A_READ == HIGH && B_READ == LOW) current_value = 3;

  last_value = current_value;
}

void loop() {
  // Intet her – alt håndteres i interrupts
}
