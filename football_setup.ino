// -------------------------------------
// Setup rod using buttons on either end
// -------------------------------------

void setup_buttons() {
  // Temp vars
  int buttonHit = 0;
  int steps = 0;

  for (int i = 0; i < STEPPER_COUNT; i++)
  {
    // --------------
    // Pin assignment
    // --------------
    
    // This assumes the connection to the Arduino is STEP-DIR-BUTTON
    // This can be changed to match the actual connection setup
    steppers[i].stepPin = (i*3) + 2;
    steppers[i].dirPin = (i*3) + 3;
    steppers[i].buttonPin = (i*3) + 4;
    pinMode(steppers[i].stepPin, OUTPUT);
    pinMode(steppers[i].dirPin, OUTPUT);
    pinMode(steppers[i].buttonPin, INPUT);

    // Reset number of steps for this rod
    steppers[i].steps = 0;

    // ------------------------------
    // Calibrate rod to position 0.0f
    // ------------------------------

    // We will move the rod to the end when DIR is set LOW
    digitalWrite(steppers[i].dirPin, LOW);

    // Temp var to store number of steps taken when finding endpoint
    // It will stop seeking when this reaches MAX_STEPS
    steps = 0;

    // Seek to the beginning (look for the button to be actuated or max steps reached)
    while (!buttonHit && steps < MAX_STEPS) {
      stepMotor(i, 4000);
      steps++;
      buttonHit = digitalRead(steppers[i].buttonPin);
    }

    // Reset temp vars for finding the other endpoint
    buttonHit = 0;
    steps = 0;

    digitalWrite(steppers[i].dirPin, HIGH);

    while (!buttonHit && steps < MAX_STEPS) {
      stepMotor(i, 4000);
      steps++;
      buttonHit = digitalRead(steppers[i].buttonPin);
    }

    steppers[i].steps = steps;

    // Reset back to position 0.0f using step count
    for (int j = 0; j < steppers[i].steps; j++) {
      stepMotor(i, 4000);
    }
    steppers[i].positionFloat = 0.0f;
    steppers[i].positionSteps = 0;

    // Alternatively set initial position to 1.0f
    //steppers[i].positionFloat = 1.0f;
    //steppers[i].positionSteps = steps;
  }
}

// --------------------------------
// Setup rod using hardcoded values
// --------------------------------

void setup_hardcoded() {
  // Pins for the steps and direction
  steppers[0].stepPin = 3;
  steppers[0].dirPin = 4;

  steppers[1].stepPin = 5;
  steppers[1].dirPin = 6;

  steppers[2].stepPin = 7;
  steppers[2].dirPin = 8;

  steppers[3].stepPin = 9;
  steppers[3].dirPin = 10;

  // Number of steps between endpoints (about 826)
  steppers[0].steps = 826;
  steppers[1].steps = 826;
  steppers[2].steps = 826;
  steppers[3].steps = 826;
  
  // Set each to output
  for (int i = 0; i < 4; i++) {
    pinMode(steppers[i].stepPin, OUTPUT);
    pinMode(steppers[i].dirPin, OUTPUT);

    digitalWrite(steppers[i].dirPin, LOW);
    for (int j = 0; j < steppers[i].steps; j++) {
      stepMotor(steppers[i].stepPin, 4000);
    }

    steppers[i].positionFloat = 0.0f;
    steppers[i].positionSteps = 0;
  }
}
