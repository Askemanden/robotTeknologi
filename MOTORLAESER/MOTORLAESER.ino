const int states[4][4] = {
  {0,1,0,-1},
  {-1,0,1,0},
  {0,-1,0,1},
  {1,0,-1,0}
};

const uint8_t ENCODER_OUT_A = 3;

const uint8_t ENCODER_OUT_B = 2;

const uint8_t LED_PIN = 4;

const uint8_t FULL_ROTATION = 64;

volatile bool led_state = LOW;

volatile uint8_t position = 0;

volatile uint8_t last_value;
volatile uint8_t current_value;

volatile long int time = 0;


void change(){
  const int A_READ = digitalRead(ENCODER_OUT_A);
  const int B_READ = digitalRead(ENCODER_OUT_B);
  if(time <= 1000){
    Serial.print("A");
    Serial.println(A_READ);
    Serial.print("B");
    Serial.println(B_READ);
  }
  last_value = current_value;

  if(A_READ == HIGH && B_READ == HIGH){
    current_value = 0;
  } else if(A_READ == LOW && B_READ == HIGH){
    current_value = 1;
  } else if(A_READ == LOW && B_READ == LOW){
    current_value = 2;
  } else if(A_READ == HIGH && B_READ == LOW){
    current_value = 3;
  }

  position += states[last_value][current_value];

  if(position>=FULL_ROTATION || position <= -FULL_ROTATION){
    led_state = !led_state;
    digitalWrite(LED_PIN,led_state);
    if(position > 0){
      position -= FULL_ROTATION;
    } else{
      position += FULL_ROTATION;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ENCODER_OUT_A, INPUT);
  pinMode(ENCODER_OUT_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_OUT_A), change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_OUT_B), change, CHANGE);

  const int A_READ = digitalRead(ENCODER_OUT_A);
  const int B_READ = digitalRead(ENCODER_OUT_B);

  if(A_READ == HIGH && B_READ == HIGH){
    current_value = 0;
  } else if(A_READ == LOW && B_READ == HIGH){
    current_value = 1;
  } else if(A_READ == LOW && B_READ == LOW){
    current_value = 2;
  } else if(A_READ == HIGH && B_READ == LOW){
    current_value = 3;
  }

  last_value = current_value;

}

void loop() {
  // put your main code here, to run repeatedly:
  time = millis();
}
