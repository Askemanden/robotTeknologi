// Licence: 🦄

// using namespace 🤫🧏‍♂️;

const uint8_t dir1 = 13, dir2 = 12, enable = 11, encoder1 = 2, encoder2 = 3;

volatile bool hall_read1 = false, hall_read2 = false;
volatile int state_now = 0, state_prior = 0, position = 0;

volatile int state_array[4][4] = {
  { 0, -1,  0,  1},
  { 1,  0, -1,  0},
  { 0,  1,  0, -1},
  {-1,  0,  1,  0}
};

const int FULL_ROTATION = 64 * 70;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(dir1, OUTPUT);  pinMode(dir2, OUTPUT);  pinMode(enable, OUTPUT);
  pinMode(encoder1, INPUT_PULLUP); pinMode(encoder2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder1), update_motor_pos, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2), update_motor_pos, CHANGE);

  digitalWrite(enable, HIGH);
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  Serial.println("Motor kører");
}

void update_motor_pos() {
  hall_read1 = digitalRead(encoder1);
  hall_read2 = digitalRead(encoder2);
  
  if (hall_read1 == HIGH && hall_read2 == HIGH) state_now = 0;
  if (hall_read1 == HIGH && hall_read2 == LOW ) state_now = 1;
  if (hall_read1 == LOW  && hall_read2 == LOW ) state_now = 2;
  if (hall_read1 == LOW  && hall_read2 == HIGH) state_now = 3;

  //position += state_array[state_now][state_prior];
  position += state_array[state_prior][state_now];
  state_prior = state_now;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (position >= FULL_ROTATION || position <= -FULL_ROTATION) {
    digitalWrite(enable, LOW);
    Serial.println("Færdig");
  }

}
