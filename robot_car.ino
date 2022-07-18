#include <Servo.h>

#define NUM_ANGLES 7

Servo servo;

unsigned char sensorAngle[NUM_ANGLES] = {60, 70, 80, 90, 100, 110, 120};
unsigned int distance[NUM_ANGLES];

/* Ultrasonic Module pins */
const int trigPin = 13;
const int echoPin = 12;

/* Servo motor pins */
const int servoPin = 11;

/* Left motor control */
const int enAPin = 6; // PWD speed control
const int in1Pin = 7; // Direction 1
const int in2Pin = 5; // Direction 2

/* Right motor control */
const int enBPin = 3; // PWD speed control
const int in3Pin = 4; // Direction 1
const int in4Pin = 2; // Direction 2

enum Motor {LEFT, RIGHT};

// Set motor speed: 255 full ahead, −255 full reverse , 0 stop
void go(enum Motor m, int speed) {
  digitalWrite(m == LEFT ? in1Pin : in3Pin, speed > 0 ? HIGH : LOW);
  digitalWrite(m == LEFT ? in2Pin : in4Pin, speed <= 0 ? HIGH : LOW);
  analogWrite(m == LEFT ? enAPin : enBPin, speed < 0 ? -speed : speed);
}

void testMotors() {
  static int speed[8] = {128, 255, 128, 0, -128, -255, -128, 0};

  go(RIGHT, 0);

  for (unsigned char i = 0; i < 8; i++ )
    go(LEFT, speed[i]), delay(200);

  for (unsigned char i = 0; i < 8; i++ )
    go(RIGHT, speed[i]), delay(200);
}

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
 
 /* Left motor control */
 pinMode(enAPin, OUTPUT);
 pinMode(in1Pin, OUTPUT);
 pinMode(in2Pin, OUTPUT);
 
 /* Right motor control */
 pinMode(enBPin, OUTPUT);
 pinMode(in3Pin, OUTPUT);
 pinMode(in4Pin, OUTPUT);

 servo.attach(servoPin);
 servo.write(90);
 
 go(LEFT, 0);
 go(RIGHT, 0);
 
 testMotors();

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
     go(LEFT, -220);
     go(RIGHT, -120);
   } 
   else {
     go(LEFT, 255);
     go(RIGHT, 255);
   }
 }
 
 delay (50);
}