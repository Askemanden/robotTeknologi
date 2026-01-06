#define PATH_LENGTH 200
#define DOUBLE_MAX 1.7976931348623157e+308
#define DOUBLE_MIN 2.2250738585072014e-308


typedef struct Vector{
  double x;
  double y;
} Vector;

typedef struct AnglesRad{
  double theta1;
  double theta2;
} AnglesRad;

typedef struct AnglesDeg{
  double theta1;
  double theta2;
} AnglesDeg;

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



AnglesRad computeIK(double L1, double L2, double Px, double Py){
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




void setup() {
  Serial.begin(9600);
  GenerateLinePath({0,10}, {-10,-10}, 0.5);

  for(int i = 0; i < PATH_LENGTH; i++){
    if(path[i].x == ENDLIST.x && path[i].y == ENDLIST.y){
      break;
    }
    Serial.print(path[i].x); Serial.print(", "); Serial.print(path[i].y); Serial.print("\n");
    delay(100);
  }
  for(int i = 0; i < PATH_LENGTH; i++){
    if(path[i].x == ENDLIST.x && path[i].y == ENDLIST.y){
      break;
    }
    AnglesRad ang = computeIK(10,10,path[i].x,path[i].y);
    Serial.print(ang.theta1); Serial.print(", "); Serial.print(ang.theta2); Serial.print("\n");
    delay(100);

  }
}


void loop() {


}

