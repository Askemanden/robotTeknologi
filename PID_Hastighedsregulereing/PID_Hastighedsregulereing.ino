// Pin definitions
uint8_t MPWMPin = 9;
uint8_t M1Pin = 10;
uint8_t M2Pin = 11;

uint8_t BUTTON_PIN = 12;

uint8_t LEDPin1 = 4;
uint8_t LEDPin2 = 5;

uint8_t HALL_SENSOR1 = 2;
uint8_t HALL_SENSOR2 = 3;

uint8_t MOTOR_SPEED_CONTROL = A0;

const int FULL_ROTATION = 960;

// PID parameters
float SystemSp = 7.0;
float PKonst = 1.0;
float PInt = 1.0;
float PDiff = 1.0;

float Error = 0.0, ErrorBefore = 0.0, DiffError = 0.0, ErrorSum = 0.0;
float MotorSp = 0.0, Hast = 0.0;
float SetPktHast = 0.0;

int MotorPWM = 0;
int MotorSetPoint = 0;

bool Flag1 = LOW;
bool Flag2 = LOW;
int iterationCounter = 0;

// Encoder variables
volatile boolean HALL_READ1, HALL_READ2;
volatile int StateNow = 0, StatePrior = 0;
volatile long position = 0;
volatile int StateArr[4][4] = {
  {0, -1, 0, 1},
  {1,  0, -1, 0},
  {0,  1, 0, -1},
  {-1, 0, 1, 0}
};

long PositionBefore = 0;
long PositionPrior = 0;
long PositionDiff = 0;

unsigned long TimeNow = 0, TimeBefore = 0;

void setup() {
  pinMode(MPWMPin, OUTPUT);
  pinMode(M1Pin, OUTPUT);
  pinMode(M2Pin, OUTPUT);

  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);

  pinMode(HALL_SENSOR1, INPUT);
  pinMode(HALL_SENSOR2, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(HALL_SENSOR1), IntProg, CHANGE);
  attachInterrupt(digitalPinToInterrupt(HALL_SENSOR2), IntProg, CHANGE);

  while (digitalRead(BUTTON_PIN)) delay(50);

  HALL_READ1 = digitalRead(HALL_SENSOR1);
  HALL_READ2 = digitalRead(HALL_SENSOR2);
  if (HALL_READ1 == HIGH && HALL_READ2 == HIGH) StatePrior = 0;
  if (HALL_READ1 == HIGH && HALL_READ2 == LOW) StatePrior = 1;
  if (HALL_READ1 == LOW && HALL_READ2 == LOW) StatePrior = 2;
  if (HALL_READ1 == LOW && HALL_READ2 == HIGH) StatePrior = 3;

  digitalWrite(M1Pin, HIGH);
  digitalWrite(M2Pin, LOW);

  TimeBefore = micros();

  Serial.begin(9600);
  Serial.println("PID Motor Control Started");
}

void IntProg() {
  HALL_READ1 = digitalRead(HALL_SENSOR1);
  HALL_READ2 = digitalRead(HALL_SENSOR2);
  if (HALL_READ1 == HIGH && HALL_READ2 == HIGH) StateNow = 0;
  if (HALL_READ1 == HIGH && HALL_READ2 == LOW) StateNow = 1;
  if (HALL_READ1 == LOW && HALL_READ2 == LOW) StateNow = 2;
  if (HALL_READ1 == LOW && HALL_READ2 == HIGH) StateNow = 3;

  position += StateArr[StateNow][StatePrior];
  StatePrior = StateNow;
}

void loop() {
  TimeNow = micros();

  if ((TimeNow - TimeBefore) >= 100000) {
    MotorSetPoint = analogRead(MOTOR_SPEED_CONTROL);
    SetPktHast = float(MotorSetPoint) / 1024.0 * (SystemSp - 4.0);

    PositionDiff = position - PositionBefore;
    Hast = float(PositionDiff) / 1920.0 * 10.0;
    PositionBefore = position;

    Error = SetPktHast - Hast;
    DiffError = Error - ErrorBefore;
    ErrorBefore = Error;

    ErrorSum += Error;
    ErrorSum = constrain(ErrorSum, 0.0, SystemSp / PInt);

    MotorSp = PKonst * Error + PDiff * DiffError + PInt * ErrorSum;
    MotorSp = constrain(MotorSp, 0.0, SystemSp);

    MotorPWM = int(MotorSp / SystemSp * 255.0);
    analogWrite(MPWMPin, MotorPWM);

    TimeBefore = TimeNow;

    iterationCounter++;
    if (iterationCounter == 10) {
      iterationCounter = 0;
      Flag1 = !Flag1;
      digitalWrite(LEDPin1, Flag1);
    }
  }

  if (abs(position - PositionPrior) >= FULL_ROTATION) {
    PositionPrior = position;
    Flag2 = !Flag2;
    digitalWrite(LEDPin2, Flag2);
  }
}
