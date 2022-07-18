#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

class MotorDriver {
private:
  // Left motor control
  int enAPin_, in1Pin_, in2Pin_;
  // Right motor control
  int enBPin_, in3Pin_, in4Pin_;
public:
  enum Motor { LEFT, RIGHT };

  MotorDriver();

  /**
   * @brief Rotate the left or right motor in any direction
   * @param m Motor you want to control, LEFT or RIGHT
   * @param speed Speed of motor
   * @details Set motor speed: 255 full ahead, −255 full reverse , 0 stop
   */
  void go(enum Motor m, int speed);

  void testMotors();
};

#endif