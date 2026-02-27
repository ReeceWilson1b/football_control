#include "PCA9685.h"
#include <Wire.h>

struct Stepper {
  // The pins on the Arduino connected to the motor and button
  unsigned int stepPin;
  unsigned int dirPin;
  unsigned int buttonPin;

  // The number of steps between positions 0.0f and 1.0f
  unsigned int steps;

  // The position of the rod in both float and steps
  float positionFloat;
  unsigned int positionSteps;
};

#define STEPPER_COUNT 4
#define SERVO_COUNT   4

#define MAX_STEPS     1000

#define HARDCODE_SETUP  1

Stepper steppers[STEPPER_COUNT];
ServoDriver servoDriver;

int currentRod = 0;

void setup() {

  // Enable serial monitor on 9600 baud
  Serial.begin(9600);

  // Enable I2C
  Wire.begin();
  servoDriver.init(0x7F);

  if (HARDCODE_SETUP)
    setup_hardcoded();
  else
    setup_buttons();
}

// Advance the motor by one step (requires the direction pin to be set beforehand)
void stepMotor(unsigned int stepPin, unsigned int delayTime) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(delayTime);
}

// Set the stepper to a certain position between 0.0f and 1.0f (edges)
void positionStepper(unsigned int id, float newPosition) {
  // Make sure id is valid
  if (id >= STEPPER_COUNT) {
    Serial.print("Invalid id ");
    Serial.print(id);
    Serial.print("\n");

    return;
  }

  // Clamp position between 0.0f and 1.0f
  if (newPosition < 0.0f)
   newPosition = 0.0f;
  else if (newPosition > 1.0f)
   newPosition = 1.0f;

  // Calculate change in position
  float deltaPos = newPosition - steppers[id].positionFloat;

  // Calculate required number of steps
  int stepCount = steppers[id].steps * deltaPos;

  // Set direction pin depending on deltaPos sign
  if (deltaPos >= 0)
    digitalWrite(steppers[id].dirPin, HIGH);
  else
    digitalWrite(steppers[id].dirPin, LOW);

  // Perform the required number of steps
  for (int step = 0; step < stepCount; step++) {
    stepMotor(steppers[id].stepPin, 4000);
  }

  // Set the position variables
  steppers[id].positionSteps += stepCount;
  if (steppers[id].positionSteps != 0)
    steppers[id].positionFloat = (float)steppers[id].positionSteps / (float)steppers[id].steps;
  else
    steppers[id].positionFloat = 0.0f;
}

void kickBall(unsigned int rod) {
  servoDriver.setAngle(rod, 45);
  delay(250);
  servoDriver.setAngle(rod, 0);
  delay(250);
}

  
void loop() {
  parse_serial();
}
