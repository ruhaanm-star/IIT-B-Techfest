
const int irPins[8] = {4, 2, 34, 35, 32, 33, 25, 26};

// Motor control pins
const int motorLeftForward = 13;
const int motorLeftBackward = 12;
const int motorRightForward = 14;
const int motorRightBackward = 27;

// Variables to store sensor readings
int sensorValues[8];

void setup() {
  // Initialize sensor pins as input
  for (int i = 0; i < 8; i++) {
    pinMode(irPins[i], INPUT);
  }

  // Initialize motor pins as output
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorLeftBackward, OUTPUT);
  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Read all sensor values
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = digitalRead(irPins[i]);
  }

  // Debug: print sensor readings
  for (int i = 0; i < 8; i++) {
    Serial.print(sensorValues[i]);
  }
  Serial.println();

  // Now using: HIGH = on the line, LOW = off the line
  bool center    = (sensorValues[3] == LOW || sensorValues[4] == LOW);
  bool leftSide  = (sensorValues[1] == LOW || sensorValues[2] == LOW);
  bool rightSide = (sensorValues[5] == LOW || sensorValues[6] == LOW );
  bool leftSharp  = (sensorValues[0] == LOW );
  bool rightSharp = (sensorValues[7] == LOW);
  bool junctionLR = (sensorValues[2] == LOW && sensorValues[5] == LOW); // adjacent center sensors see line

  // Priority: center -> junction -> right-only -> left-only -> both -> stop/search
    if (rightSharp) {
    sharpRight();
  } else if (leftSharp) {
    sharpLeft();
  } 
    else if (rightSide) {
    turnRight();
  } else if (leftSide) {
    turnLeft();
  } 
    else if (center) {
    moveForward();
  } else if (junctionLR) {
    // choose preferred junction behavior; here prefer right
    turnRight();
  } else if (leftSide && rightSide) {
    // both sides see line (ambiguous) — treat as junction; prefer right
    turnRight();
  } else {
    // No sensor sees the line: stop or implement search routine
    stopMotors();
  }

  delay(50);
}

void moveForward() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
}

void turnLeft() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
}

void turnRight() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, HIGH);
}
void sharpRight() {
  moveForward();
  delay(300);
  turnRight();
  delay(300);
  
}
void sharpLeft() {
  moveForward();
  delay(300);
  turnLeft();
  delay(300);
  
}

void stopMotors() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, LOW);
}
