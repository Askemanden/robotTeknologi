enum Color {
  BLACK,
  NOT_BLACK
};

typedef struct Sensor {
  unsigned long last_line_sighting;
  const int dark_value;
  const int max_variance;
  const uint8_t pin;
} Sensor;

Sensor SensorInit(const uint8_t pin, const int dark_value, const int max_variance){
  pinMode(pin, INPUT);
  return {pin, dark_value, max_variance, 0};
}

Color SensorRead(Sensor *sensor){
  if(analogRead(sensor->pin) > (sensor->dark_value+sensor->max_variance)){
    return NOT_BLACK;
  } else{
    return BLACK;
  }
}

typedef struct Wheels {
  const uint8_t left;
  const uint8_t right;
} Wheels;



typedef struct Car {
  const Sensor *sensors;
  const Wheels wheels;
} Car;


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
