#include "activateMotor.h"

const int motorPin = 5;

void activateMotor() {
  Serial.println("Alarm Activated! Wake up!");

 digitalWrite(motorPin, HIGH); // Turn on the motor
 delay(5000);                  // Keep the motor on for 5 seconds
 digitalWrite(motorPin, LOW);  // Turn off the motor
 digitalWrite(motorPin, HIGH); // Turn on the motor
 delay(5000);                  // Keep the motor on for 5 seconds
 digitalWrite(motorPin, LOW);  // Turn off the motor
 digitalWrite(motorPin, HIGH); // Turn on the motor
 delay(5000);                  // Keep the motor on for 5 seconds
 digitalWrite(motorPin, LOW);  // Turn off the motor
 digitalWrite(motorPin, HIGH); // Turn on the motor
 delay(5000);                  // Keep the motor on for 5 seconds
 digitalWrite(motorPin, LOW);  // Turn off the motor
 digitalWrite(motorPin, HIGH); // Turn on the motor
 delay(5000);                  // Keep the motor on for 5 seconds
 digitalWrite(motorPin, LOW);  // Turn off the motor
 digitalWrite(motorPin, HIGH); // Turn on the motor
 delay(5000);                  // Keep the motor on for 5 seconds
 digitalWrite(motorPin, LOW);  // Turn off the motor
}