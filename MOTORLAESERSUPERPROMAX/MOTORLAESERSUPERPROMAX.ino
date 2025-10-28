uint8_t MOTOR_SPEED_CONTROL = A0;

uint8_t MPWMPin = 9;
uint8_t M1Pin = 10;
uint8_t M2Pin = 11;

uint8_t HALL_SENSOR1 = 2;
uint8_t HALL_SENSOR2 = 3;

uint8_t LEDPin1 = 4;
uint8_t LEDPin2 = 5;

uint8_t BUTTON_PIN = 12;

const int FULL_ROTATION = 960;

int number_of_iterations_since_last_reset = 0;

boolean Flag1 = LOW;
boolean Flag2 = LOW;

int MotorSetPoint;
int MotorSignal;

volatile boolean HALL_READ1, HALL_READ2;
volatile int StateNow, StatePrior;
volatile long position = 0;
volatile int ArrIndex;
volatile int StateArr[4][4]={
  {0,-1,0,1},
  {1,0,-1,0},
  {0,1,0,-1},
  {-1,0,1,0}
};

long StepPrior = 0;
int StepSinceLastReset;

bool direction = HIGH;

void setup() {
  
  pinMode(MPWMPin, OUTPUT);
  pinMode(M1Pin, OUTPUT);
  pinMode(M2Pin, OUTPUT);
  
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  
  pinMode(HALL_SENSOR1, INPUT);
  pinMode(HALL_SENSOR2, INPUT);
  
  pinMode(BUTTON_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(HALL_SENSOR1),IntProg,CHANGE);
  attachInterrupt(digitalPinToInterrupt(HALL_SENSOR2),IntProg,CHANGE);

  while(digitalRead(BUTTON_PIN))delay(50);

  HALL_READ1 = digitalRead(HALL_SENSOR1);
  HALL_READ2 = digitalRead(HALL_SENSOR2);
  
  if (HALL_READ1 == HIGH && HALL_READ2 == HIGH) StatePrior = 0;
  if (HALL_READ1 == HIGH && HALL_READ2 == LOW) StatePrior = 1;
  if (HALL_READ1 == LOW && HALL_READ2 == LOW) StatePrior = 2;
  if (HALL_READ1 == LOW && HALL_READ2 == HIGH) StatePrior = 3;   

  digitalWrite(M1Pin, !direction);
  digitalWrite(M2Pin, direction);


  Serial.begin(9600);
  Serial.println("START");
}

void IntProg() {
  HALL_READ1 = digitalRead(HALL_SENSOR1);
  HALL_READ2 = digitalRead(HALL_SENSOR2);
  if (HALL_READ1 == HIGH && HALL_READ2 == HIGH) StateNow = 0;
  if (HALL_READ1 == HIGH && HALL_READ2 == LOW) StateNow = 1;
  if (HALL_READ1 == LOW && HALL_READ2 == LOW) StateNow = 2;
  if (HALL_READ1 == LOW && HALL_READ2 == HIGH) StateNow = 3;

  position = position + StateArr[StateNow][StatePrior];
  StatePrior = StateNow;
}

void loop() {

  if(!digitalRead(BUTTON_PIN)){
    direction = !direction;
    digitalWrite(M1Pin, !direction);
    digitalWrite(M2Pin, direction);
  }

  MotorSetPoint = analogRead(MOTOR_SPEED_CONTROL);
  MotorSignal = int(MotorSetPoint / 4.0);
    
  analogWrite(MPWMPin,MotorSignal);

  StepSinceLastReset = position - StepPrior;
  if (StepSinceLastReset >= FULL_ROTATION || StepSinceLastReset <= -FULL_ROTATION) {
    StepPrior = position;
    StepSinceLastReset = 0;
    Flag1 = !Flag1;
    digitalWrite(LEDPin1,Flag1);
  }

  number_of_iterations_since_last_reset += 1;
  if (number_of_iterations_since_last_reset == 10000) {
    number_of_iterations_since_last_reset = 0;
    Serial.print(" StepSinceLastReset: ");
    Serial.println(StepSinceLastReset);
    Flag2 = !Flag2;
    digitalWrite(LEDPin2,Flag2);
  }
}
