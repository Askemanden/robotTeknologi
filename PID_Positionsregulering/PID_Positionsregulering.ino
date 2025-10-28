uint8_t MPWMPin = 9;
uint8_t M1Pin = 10;
uint8_t M2Pin = 11;

uint8_t BUTTON_PIN = 12;

uint8_t LEDPin1 = 4;
uint8_t LEDPin2 = 5;

uint8_t HALL_SENSOR1 = 2;
uint8_t HALL_SENSOR2 = 3;

uint8_t POSITION_CONTROL = A0;

const long FULL_ROTATION = 1000;
const long ANALOG_TO_STEPS = 1920;

float PKonst = 0.3;
float PInt = 0.0;
float PDiff = 0.25;

float Error = 0.0;
float ErrorBefore = 0.0;
float DiffError = 0.0;
float ErrorSum = 0.0;

float SetPktPos = 0.0;

int MotorPWM = 0;
int AnalogSet = 0;

bool Flag1 = LOW;
bool Flag2 = LOW;
int iterationCounter = 0;

volatile boolean HALL_READ1 = LOW;
volatile boolean HALL_READ2 = LOW;
volatile int StateNow = 0;
volatile int StatePrior = 0;
volatile long position = 0;

volatile int StateArr[4][4] = {
  { 0, -1,  0,  1},
  { 1,  0, -1,  0},
  { 0,  1,  0, -1},
  {-1,  0,  1,  0}
};

long PositionBefore = 0;
long PositionPrior = 0;

unsigned long TimeNow = 0;
unsigned long TimeBefore = 0;

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
  if (HALL_READ1 == HIGH && HALL_READ2 == LOW ) StatePrior = 1;
  if (HALL_READ1 == LOW  && HALL_READ2 == LOW ) StatePrior = 2;
  if (HALL_READ1 == LOW  && HALL_READ2 == HIGH) StatePrior = 3;

  digitalWrite(M1Pin, HIGH);
  digitalWrite(M2Pin, LOW);

  TimeBefore = micros();

  ErrorBefore = 0.0;
  ErrorSum = 0.0;

  Serial.begin(9600);
  Serial.println("Position control (refactored) started");
}

void IntProg() {
  HALL_READ1 = digitalRead(HALL_SENSOR1);
  HALL_READ2 = digitalRead(HALL_SENSOR2);

  if (HALL_READ1 == HIGH && HALL_READ2 == HIGH) StateNow = 0;
  if (HALL_READ1 == HIGH && HALL_READ2 == LOW ) StateNow = 1;
  if (HALL_READ1 == LOW  && HALL_READ2 == LOW ) StateNow = 2;
  if (HALL_READ1 == LOW  && HALL_READ2 == HIGH) StateNow = 3;

  position += StateArr[StateNow][StatePrior];
  StatePrior = StateNow;
}

void loop() {
  TimeNow = micros();

  if ((TimeNow - TimeBefore) >= 100000UL) {
    AnalogSet = analogRead(POSITION_CONTROL);
    SetPktPos = float(AnalogSet) / 1024.0 * float(ANALOG_TO_STEPS) - float(ANALOG_TO_STEPS) / 2.0;

    Error = SetPktPos - float(position);

    DiffError = Error - ErrorBefore;
    ErrorBefore = Error;

    ErrorSum += Error;
    if (ErrorSum >  500.0) ErrorSum =  500.0;
    if (ErrorSum < -500.0) ErrorSum = -500.0;

    MotorPWM = int(PKonst * Error + PDiff * DiffError + PInt * ErrorSum);

    if (MotorPWM > 0) {
      if (MotorPWM > 254) MotorPWM = 254;
      digitalWrite(M1Pin, HIGH);
      digitalWrite(M2Pin, LOW);
      analogWrite(MPWMPin, MotorPWM);
    } else if (MotorPWM < 0) {
      if (MotorPWM < -254) MotorPWM = -254;
      digitalWrite(M1Pin, LOW);
      digitalWrite(M2Pin, HIGH);
      analogWrite(MPWMPin, -MotorPWM);
    } else {
      analogWrite(MPWMPin, 0);
    }

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
