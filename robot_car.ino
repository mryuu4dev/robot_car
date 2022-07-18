#include <Servo.h>
#include "motor_driver.h"

#define NUM_ANGLES 7

Servo servo;

unsigned char sensorAngle[NUM_ANGLES] = {60, 70, 80, 90, 100, 110, 120};
unsigned int distance[NUM_ANGLES];

/* Ultrasonic Module pins */
const int trigPin = 13;
const int echoPin = 12;

/* Servo motor pins */
const int servoPin = 11;

MotorDriver motorDriver = MotorDriver();

double readDistance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  unsigned long period = pulseIn(echoPin, HIGH);

  return period * 343 / 2000;
}

void readNextDistance() {
  static unsigned char angleIndex = 0;
  static signed char step = 1;

  distance[angleIndex] = readDistance();
  angleIndex += step;
  
  if (angleIndex == NUM_ANGLES - 1) step = -1;
  else if (angleIndex == 0) step = 1;
  
  servo.write(sensorAngle[angleIndex]);
}

void setup() {
  
  /* Ultrasonic Module pins */
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);

  servo.attach(servoPin);
  servo.write(90);

  motorDriver.testMotors();

  servo.write(sensorAngle[0]);
  delay(200);

  for (unsigned char i = 0; i < NUM_ANGLES; i++) {
    readNextDistance();
    delay(200);
  }
}

void loop() {
  readNextDistance();

  unsigned char tooClose = 0;
  for (unsigned char i = 0; i < NUM_ANGLES; i++) {
    if (distance[i] < 300) {
      tooClose = 1;
    }
    if (tooClose) {
      motorDriver.go(MotorDriver::Motor::LEFT, -220);
      motorDriver.go(MotorDriver::Motor::RIGHT, -120);
    } 
    else {
      motorDriver.go(MotorDriver::Motor::LEFT, 255);
      motorDriver.go(MotorDriver::Motor::RIGHT, 255);
    }
  }

  delay (50);
}