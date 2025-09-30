const uint8_t DIR_LEFT = 10;
const uint8_t PWM_LEFT = 6;
const uint8_t DIR_RIGHT = 5;
const uint8_t PWM_RIGHT = 9;                                                                  



void setup() {
  // put your setup code here, to run once:
  pinMode(DIR_RIGHT, OUTPUT);
  pinMode(DIR_LEFT, OUTPUT);
  pinMode(PWM_RIGHT, OUTPUT);
  pinMode(PWM_LEFT, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(DIR_LEFT,HIGH);
  digitalWrite(DIR_RIGHT,LOW);
  analogWrite(PWM_LEFT, 150);
  analogWrite(PWM_RIGHT, 150);
}
