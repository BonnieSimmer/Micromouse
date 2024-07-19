#define IR_C 4
#define IR_TL 2
#define IR_TR 7
#define IR_BL 11
#define IR_BR 12
#define ML2 5
#define ML1 6  //fwd
#define MR1 9  //fwd
#define MR2 10

bool front;
bool leftTop;
bool leftBottom;
bool rightTop;
bool rightBottom;

bool speedInput = 0;

bool frontObstacle;
bool leftTopObstacle;
bool leftBottomObstacle;
bool rightTopObstacle;
bool rightBottomObstacle;


const int timeCorner = 1050;  //time it takes to make a 90 degree angle around corners;

int speedRight = 160;
int speedLeft = 179;
float MultiplerR;
float MultiplerL;
//Declare all the functions being used here:
void moveForward();
void turnRight();
void turnLeft();
void AdjustRight();
void adjustLeft();
void stopMotors();

void setup() {
  // put your setup code here, to run once:
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);

  pinMode(IR_C, INPUT);
  pinMode(IR_TR, INPUT);
  pinMode(IR_BR, INPUT);
  pinMode(IR_TL, INPUT);
  pinMode(IR_BL, INPUT);

  Serial.begin(9600);

  //Send Speed over bluetooth;
  while (!speedInput) {
    if (Serial.available() > 0)  // if data is available to read
    {
      speedRight = Serial.parseInt();
      speedLeft = Serial.parseInt();
      MultiplerR = Serial.parseFloat();
      MultiplerL = Serial.parseFloat();
      speedInput = Serial.parseInt();
    }
  }
}
void loop() {

  //Read sensor values
  front = digitalRead(IR_C);
  leftTop = digitalRead(IR_TL);
  leftBottom = digitalRead(IR_BL);
  rightTop = digitalRead(IR_TR);
  rightBottom = digitalRead(IR_BR);
  
  // Print sensor values for debugging
  Serial.print("Front: ");
  Serial.print(front);
  Serial.print(" LeftTop: ");
  Serial.print(leftTop);
  Serial.print(" LeftBottom: ");
  Serial.print(leftBottom);
  Serial.print(" RightTop: ");
  Serial.print(rightTop);
  Serial.print(" RightBottom: ");
  Serial.println(rightBottom);

  // Check for obstacles
  frontObstacle = (front == 0);
  leftTopObstacle = (leftTop == 0);
  leftBottomObstacle = (leftBottom == 0);
  rightTopObstacle = (rightTop == 0);
  rightBottomObstacle = (rightBottom == 0);

  //Navigating
   if (frontObstacle) {
    if (!leftBottomObstacle) {
      turnLeft();
    } else if (!rightBottomObstacle) {
      turnRight();
    } else {
      stopMotors();
      while (!restart) {
        restart= Serial.parseInt();
      }
      restart=0;
    }
  } else {
    // Adjust to stay centered
    if (leftTopObstacle) {
      adjustRight();
    } else if (rightTopObstacle) {
      adjustLeft();
    } else {
      moveForward();
    }
  }

 // delay(100);  // Short delay for sensor reading stabilization
}

void turnLeft() {
  Serial.println("Turning left");
  digitalWrite(ML2, LOW);
  digitalWrite(ML1, LOW);
  digitalWrite(MR2, LOW);
  analogWrite(MR1, speedRight);

  delay(timeCorner);
}

void adjustLeft() {
 Serial.println("Adjusting left");
  digitalWrite(ML2, LOW);
  digitalWrite(ML1, LOW);
  digitalWrite(MR2, LOW);
  analogWrite(MR1, speedRight * MultiplerL);
}

void turnRight() {
  Serial.println("Turning right");
  digitalWrite(ML2, LOW);
  analogWrite(ML1, speedLeft);
  digitalWrite(MR2, LOW);
  digitalWrite(MR1, LOW);

  delay(timeCorner);
}

void adjustRight() {
  Serial.println("Adjusting right");
  digitalWrite(ML2, LOW);
  analogWrite(ML1, speedLeft * MultiplerR);
  digitalWrite(MR2, LOW);
  digitalWrite(MR1, LOW);
}

void moveForward() {
  Serial.println("Forward");
  analogWrite(ML1, speedLeft);
  digitalWrite(ML2, LOW);
  digitalWrite(MR2, LOW);
  analogWrite(MR1, speedRight);
}

void stopMotors() {
  Serial.println("Done");
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, LOW);
  digitalWrite(MR2, LOW);
  digitalWrite(MR1, LOW);
}

