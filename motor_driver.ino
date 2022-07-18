#include "motor_driver.h"

MotorDriver::MotorDriver() {
  enAPin_ = 6, in1Pin_ = 7, in2Pin_ = 5;
  enBPin_ = 3, in3Pin_ = 4, in4Pin_ = 2;

  pinMode(enAPin_, OUTPUT);
  pinMode(in1Pin_, OUTPUT);
  pinMode(in2Pin_, OUTPUT);

  pinMode(enBPin_, OUTPUT);
  pinMode(in3Pin_, OUTPUT);
  pinMode(in4Pin_, OUTPUT);

  MotorDriver::go(LEFT, 0);
  MotorDriver::go(RIGHT, 0);
}

void MotorDriver::go(enum Motor m, int speed) {
  digitalWrite(m == LEFT ? in1Pin_ : in3Pin_, speed > 0 ? HIGH : LOW);
  digitalWrite(m == LEFT ? in2Pin_ : in4Pin_, speed <= 0 ? HIGH : LOW);
  analogWrite(m == LEFT ? enAPin_ : enBPin_, speed < 0 ? -speed : speed);
}

void MotorDriver::testMotors() {
  int speed[8] = { 128, 255, 128, 0, -128, -255, -128, 0 };

  MotorDriver::go(MotorDriver::Motor::RIGHT, 0);

  for (int i = 0; i < 8; i++) {
    MotorDriver::go(MotorDriver::Motor::LEFT, speed[i]), delay(200);
  }
  for (int i = 0; i < 8; i++) {
    MotorDriver::go(MotorDriver::Motor::RIGHT, speed[i]), delay(200);
  }
}