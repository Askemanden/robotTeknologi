struct Sensor;

typedef struct Sensor {
  uint8_t pin;
  int sensitivity;
  int black_value;
  unsigned long last_line_sighting;
} Sensor;

Sensor sensorInit(uint8_t pin) {
  pinMode(pin, INPUT);
  int readings_sum = 0;
  for (int i = 0; i < 20; i++) {
    readings_sum += analogRead(pin);
    delay(10);
  }
  Serial.println(int(readings_sum / 20));

  return { pin, 100, int(readings_sum / 20), 0 };
}

const uint8_t DIR_LEFT = 10;
const uint8_t PWM_LEFT = 6;
const uint8_t DIR_RIGHT = 5;
const uint8_t PWM_RIGHT = 9;

Sensor SENSORS[4];

const uint8_t ON_BUTTON = 2;

const uint8_t BASE_MOTOR_POWER = 200;

const float MAX_SPEED = 0.6;
const float MEDIUM_SPEED = 0.2;
const float LOW_SPEED = 0.05;
const float NO_SPEED = 0;

bool read(Sensor* sensor) {
  int reading = analogRead(sensor->pin);
  bool is_black = reading < (sensor->sensitivity + sensor->black_value);
  if(is_black){
    sensor->last_line_sighting = millis();
  }
  return is_black;
}

uint8_t readAll() {
  bool readings[4];
  for (int i = 0; i < 4; i++) {
    readings[i] = read(&SENSORS[i]);
  }
  uint8_t result = 0;
  for (int i = 0; i < 4; i++) {
    if (readings[i]) {
      result += (1 << i);
    }
  }
  return result;
}

uint8_t fixDiscontinuousReadings(uint8_t value) {
  value &= 0b1111;
  
  uint8_t left = ((value & 0b1100) >> 2);  // bits 3 and 2
  uint8_t right = (value & 0b0011);        // bits 1 and 0

  uint8_t leftCount = 0;
  uint8_t rightCount = 0;
  for (uint8_t i = 0; i < 2; i++) {
    if (left & (1 << i)) leftCount++;
    if (right & (1 << i)) rightCount++;
  }

  uint8_t totalCount = leftCount + rightCount;

  if(totalCount == 0) return value;

  if (totalCount == 1) return value;

  if (totalCount == 4) return value;

  if (value == 0b1001 || value == 0b1010 || value == 0b0101) {
    return 0b0110;
  }

  if (leftCount > rightCount) {
    return 0b1100;
  } else if (rightCount > leftCount) {
    return 0b0011;
  } else {
    return 0b0110;
  }
}

void turn(float left_ratio, float right_ratio) {
  analogWrite(PWM_LEFT, int(left_ratio * BASE_MOTOR_POWER*1.2));
  analogWrite(PWM_RIGHT, int(right_ratio * BASE_MOTOR_POWER));
}

void setup() {
  // put your setup code here, to run once:
  pinMode(DIR_RIGHT, OUTPUT);
  pinMode(DIR_LEFT, OUTPUT);
  pinMode(PWM_RIGHT, OUTPUT);
  pinMode(PWM_LEFT, OUTPUT);
  pinMode(ON_BUTTON, INPUT);
  Serial.begin(9600);

  SENSORS[0] = sensorInit(A1);
  SENSORS[1] = sensorInit(A2);
  SENSORS[2] = sensorInit(A3);
  SENSORS[3] = sensorInit(A4);
  delay(10000);
  digitalWrite(DIR_RIGHT, LOW);
  digitalWrite(DIR_LEFT, HIGH);
}

void loop() {
  uint8_t reads = readAll();
  reads = fixDiscontinuousReadings(reads);
  if(reads == 0b1111 || reads == 0b0110){
    turn(MAX_SPEED,MAX_SPEED);
  } else if(reads == 0b1000){
    turn(LOW_SPEED,MAX_SPEED);
  } else if(reads == 0b0001){
    turn(MAX_SPEED,LOW_SPEED);
  } else if(reads == 0b0100 || reads == 0b1100){
    turn(MEDIUM_SPEED,MAX_SPEED);
  }  else if(reads == 0b0010 || reads == 0b0011){
    turn(MAX_SPEED,MEDIUM_SPEED);
  } else{
    bool line_is_right = SENSORS[3].last_line_sighting > SENSORS[0].last_line_sighting || SENSORS[2].last_line_sighting > SENSORS[0].last_line_sighting && SENSORS[2].last_line_sighting > SENSORS[1].last_line_sighting || SENSORS[3].last_line_sighting > SENSORS[1].last_line_sighting;
    if(line_is_right){
      turn(MEDIUM_SPEED,NO_SPEED);
    } else{
      turn(NO_SPEED,MEDIUM_SPEED);
    }
  }
  delay(50);
}