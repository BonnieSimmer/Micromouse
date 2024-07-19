// Micromouse Competiton Luban Workshop ASU Nour Ali AbdelRahman


#define IR_C 4
#define IR_TL 2
#define IR_TR 7
#define IR_BL 11
#define IR_BR 12
#define ML2 5
#define ML1 6  //fwd
#define MR1 9  //fwd
#define MR2 10

 bool obstacleCenter;
 bool obstacleBackLeft;
 bool obstacleBackRight;
 bool obstacleTopLeft;
 bool obstacleTopRight;

int speedRight;
int speedLeft;

float multiplierL;
float multiplierR;

//Declare all the functions being used here:
void deviateRight();
void Right();
void deviateLeft();
void Left();
void forward();
void stopAll();
void centering();

bool speedInput;

void setup() {
  pinMode(IR_C, INPUT);
  pinMode(IR_TL, INPUT);
  pinMode(IR_TR, INPUT);
  pinMode(IR_BL, INPUT);
  pinMode(IR_BR, INPUT);
  
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);

  Serial.begin(9600);

    while (!speedInput) {
    if (Serial.available() > 0)  // if data is available to read
    {
      speedRight = Serial.parseInt();
      speedLeft = Serial.parseInt();
      multiplierR= Serial.parseFloat();
      multiplierL= Serial.parseFloat();
      speedInput = Serial.parseInt();
    }
  }
  centering();
}

void loop() {
  obstacleCenter = digitalRead(IR_C) == LOW;
  obstacleTopLeft = digitalRead(IR_TL) == LOW;
  obstacleTopRight = digitalRead(IR_TR) == LOW;
  obstacleBackLeft = digitalRead(IR_BL) == LOW;
  obstacleBackRight = digitalRead(IR_BR) == LOW;

// CENTERING
  if (!obstacleCenter) {
    if (obstacleTopLeft && obstacleTopRight) {
      // Move forward
      forward();
    } else if (obstacleTopLeft && !obstacleTopRight) {
      // Deviate right
      Right();
    } else if (!obstacleTopLeft && obstacleTopRight) {
      // Deviate left
      Left();
    } else if (!obstacleTopLeft && !obstacleTopRight) {
      // Move forward
     forward();
    }
  }
  // NAVIGATING
   else {
 if (obstacleTopLeft && !obstacleTopRight) {
      // Deviate right
      deviateRight();
    } else if (!obstacleTopLeft && obstacleTopRight) {
      // Deviate left
      deviateLeft();
    } else if (!obstacleTopLeft && !obstacleTopRight) {
      //choose priority
      deviateLeft();
    }
      else if (obstacleTopLeft && obstacleTopRight) {
      
      stopAll();
    }
  }
}

void forward() {
  Serial.println("Forward");
  centering();
  analogWrite(ML1, speedLeft);
  //digitalWrite(ML2, LOW);
  //digitalWrite(MR2, LOW);
  analogWrite(MR1, speedRight);
}


void Left() {
  Serial.println("Centering Left");
  //digitalWrite(ML2, LOW);
  digitalWrite(ML1, LOW);
  //digitalWrite(MR2, LOW);
  analogWrite(MR1, speedRight*multiplierL);
}
void Right() {
  Serial.println("Centering Right");
  analogWrite(ML1, speedLeft*multiplierR);
  //digitalWrite(ML2, LOW);
  //digitalWrite(MR2, LOW);
  digitalWrite(MR1, LOW);
}
void centering() {
  bool exitCentering = false;  // Flag to indicate when to exit the centering loop

  while (!exitCentering) {
    if ((digitalRead(IR_TR) == 1)&&(digitalRead(IR_C) == HIGH)) {
      Right();
    } else if ((digitalRead(IR_TL) == 1)&&(digitalRead(IR_C) == HIGH)) {
      Left();
    } else {
      exitCentering = true;  // Exit the loop when no obstacle detected on either side
    }
    delay(10);  // Add a small delay to allow the car to adjust its direction
  }
}
void stopAll() {
  Serial.println("Done");
  digitalWrite(ML1, LOW);
  //digitalWrite(ML2, LOW);
  //digitalWrite(MR2, LOW);
  digitalWrite(MR1, LOW);
 // delay(1000);
}
void deviateLeft() {
  Serial.println("Left");
  //digitalWrite(ML2, LOW);
  digitalWrite(ML1, LOW);
  //digitalWrite(MR2, LOW);
  analogWrite(MR1, speedRight);
}
void deviateRight() {
  Serial.println("Right");
  analogWrite(ML1, speedLeft);
  //digitalWrite(ML2, LOW);
  //digitalWrite(MR2, LOW);
  digitalWrite(MR1, LOW);
}
