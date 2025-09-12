const uint8_t inputs[] = {A1,A2,A3,A4};

const size_t inputLength = 4;

void setup() {
  for(int i = 0; i<inputLength; i++){
    pinMode(inputs[i], INPUT);
  }
  Serial.begin(9600);
}
  
void loop() {
  for(int i = 0; i<inputLength; i++){
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(analogRead(inputs[i]));
  }
  Serial.println();
  Serial.println();
  delay(1000);
}