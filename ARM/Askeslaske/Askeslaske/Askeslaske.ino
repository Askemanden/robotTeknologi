// Licence: 🦄

#include <math.h>
#include <Servo.h>

#define PATH_LENGTH 100
#define DOUBLE_MAX 1.7976931348623157e+10
#define DOUBLE_MIN 2.2250738585072014e-10
//#define PI 3.1415926535897932385

double lower_arm_length = 20, upper_arm_length = 18;

typedef struct Vector{
  double x;
  double y;
} Vector;

typedef struct Angles{
  double theta1;
  double theta2;
} AnglesRad, AnglesDeg;

Vector VectorAdd(Vector a, Vector b){
  return (Vector){a.x+b.x, a.y+b.y};
}

Vector VectorMul(Vector a, double scalar){
  return (Vector){a.x*scalar, a.y*scalar};
}

Vector VectorDiv(Vector a, double scalar){
  return (Vector){a.x/scalar, a.y/scalar};
}

double VectorLength(Vector a){
  return sqrt(a.x*a.x+a.y*a.y);
}

Vector VectorNormalize(Vector a){
  return VectorDiv(a, VectorLength(a));
}

const Vector ENDLIST = {.x = DOUBLE_MAX, .y = DOUBLE_MIN};

Vector path[PATH_LENGTH] = {};

// Motorvariabler
volatile bool hall_read1 = false, hall_read2 = false;
volatile int state_now = 0, state_prior = 0, current_position = 0;
volatile int state_array[4][4] = {
  { 0, -1,  0,  1},
  { 1,  0, -1,  0},
  { 0,  1,  0, -1},
  {-1,  0,  1,  0}
};
const int motor_speed = 50,         // Hastigheden motoren kører i.
          FULL_ROTATION = 64 * 100; // antal steps til en fuld rotation.


int nice_position = 0,              // Den ønskede position for motoren i trin.
    threshold = 20;                 // Hvor mange trin motoren er tilladt at dreje, før den korrigeres.
// Rotation med uret giver positive trin.

// Pins
const uint8_t clockwise = 13, counterclockwise = 12, enable = 11, encoder1 = 2, encoder2 = 3;

Servo my_servo;

void turn(bool cw)
{
  digitalWrite(clockwise, cw);
  digitalWrite(counterclockwise, !cw);
  analogWrite(enable, motor_speed);
}

// Vektorer
Vector start = {5, 5};
Vector end = {15, 15};

AnglesRad computeIK(double Px, double Py, double L1 = lower_arm_length, double L2 = upper_arm_length){
  double r = sqrt(Px * Px + Py * Py);
  double theta2 = acos( (- (L1 * L1) - (L2 * L2) + r * r) / (2 * L1 * L2) );
  double phi = atan2(Py,Px);

  double theta1 = phi - atan2(L2*sin(theta2), (L1 + L2*cos(theta2)));

  AnglesRad result = {theta1, theta2};
  return result;
}

void GenerateLinePath(Vector start, Vector end, double stepDist) {
    Vector diff = VectorAdd(end, VectorMul(start, -1));   // end - start
    double totalDist = VectorLength(diff);

    if (stepDist <= 0) {
        Serial.println("ERROR: stepDist must be > 0");
        return;
    }

    if (totalDist == 0) {
        Serial.println("ERROR: start and end are identical");
        return;
    }

    Vector direction = VectorNormalize(diff);
    
    int index = 0;
    double traveled = 0.0;
    while (traveled <= totalDist) {
        // ERROR if we run out of space
        if (index >= PATH_LENGTH) {
            Serial.println("ERROR: path[] overflow");
            return;
        }
        path[index] = VectorAdd(start, VectorMul(direction, traveled));
        traveled += stepDist;
        index++;
    }
    // Add final endpoint
    if (index >= PATH_LENGTH) {
        Serial.println("ERROR: path[] overflow");
        return;
    }
    path[index] = end;
    index++;
    // Add ENDLIST
    if (index >= PATH_LENGTH) {
        Serial.println("ERROR: path[] overflow");
        return;
    }
    path[index] = ENDLIST;
}

// Tæller antal trin som motoren har kørt, og hvad retning.
void update_motor_pos()
{
  hall_read1 = digitalRead(encoder1);
  hall_read2 = digitalRead(encoder2);
  
  if (hall_read1 == HIGH && hall_read2 == HIGH) state_now = 0;
  if (hall_read1 == HIGH && hall_read2 == LOW ) state_now = 1;
  if (hall_read1 == LOW  && hall_read2 == LOW ) state_now = 2;
  if (hall_read1 == LOW  && hall_read2 == HIGH) state_now = 3;

  current_position += state_array[state_prior][state_now];
  state_prior = state_now;
}

void setup() {
  Serial.begin(9600);
  pinMode(clockwise, OUTPUT);  pinMode(counterclockwise, OUTPUT);  pinMode(enable, OUTPUT);
  pinMode(encoder1, INPUT_PULLUP); pinMode(encoder2, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(encoder1), update_motor_pos, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2), update_motor_pos, CHANGE);

  analogWrite(enable, 0);
  digitalWrite(clockwise, LOW);
  digitalWrite(counterclockwise, LOW);

  my_servo.attach(5);

  GenerateLinePath(start, end, 0.5);
}

double nicepos(double angle)
{
  return(angle * FULL_ROTATION/(2*PI));
}

int set_motor_steps()
{
  static unsigned int index = 0;

  if (abs(current_position - nice_position) < threshold)
  {
    if (path[index].x == ENDLIST.x || path[index].y == ENDLIST.y)
    {
      return 0;
    }
    AnglesRad vinkler = computeIK(path[index].x , path[index].y);
    index++;
    nice_position = nicepos(vinkler.theta1);
    servo_motor_turn(vinkler.theta2);

    Serial.print(vinkler.theta1);
    Serial.print(",");
    Serial.println(vinkler.theta2);
  }
  return 0;
}

int dc_motor_turn()
{
  int error = current_position - nice_position;
  //Serial.println(error);
  if (threshold <= 0)
  {
    Serial.println("Mongol!, Fejl i threshold værdi");
    return 1;
  }
  if (error >= threshold || error <= -threshold)
  {
    turn(bool(( (error / abs(error)) + 1) / 2));
  } else
  {
    analogWrite(enable, 0);
  }
  return 0;
}

int servo_motor_turn(double angle)
{
  my_servo.write(int(angle*(180/PI)));
  return 0;
}

void loop()
{
  set_motor_steps();
  dc_motor_turn();
}
