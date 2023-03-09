#include <Wire.h>
#include <MPU6050.h>

// Define the motor pins and PWM values
const int MOTOR1_PIN = 5;
const int MOTOR2_PIN = 6;
const int PWM_VALUE = 255;

// Define the IR Sharp sensor pins
const int SHARP_FRONT_PIN = A0;
const int SHARP_LEFT_FRONT_PIN = A1;
const int SHARP_LEFT_BACK_PIN = A2;
const int SHARP_RIGHT_FRONT_PIN = A3;
const int SHARP_RIGHT_BACK_PIN = A4;

// Define the target angle and PID parameters
const int TARGET_ANGLE = 0;
const double KP = 0.1;
const double KI = 0.1;
const double KD = 0.1;
double integral = 0;
double derivative = 0;
double last_error = 0;

void setup() {
  // Initialize the serial communication, the MPU6050 sensor, and the motor pins
  Serial.begin(9600);
  Wire.begin();
  pinMode(MOTOR1_PIN, OUTPUT);
  pinMode(MOTOR2_PIN, OUTPUT);

  // Initialize the IR Sharp sensor pins as inputs
  pinMode(SHARP_FRONT_PIN, INPUT);
  pinMode(SHARP_LEFT_FRONT_PIN, INPUT);
  pinMode(SHARP_LEFT_BACK_PIN, INPUT);
  pinMode(SHARP_RIGHT_FRONT_PIN, INPUT);
  pinMode(SHARP_RIGHT_BACK_PIN, INPUT);
}

void loop() {
  // Read the current angle from the gyro sensor
  int16_t ax, ay, az, gx, gy, gz;
  MPU6050 mpu;
  mpu.initialize();
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  double angle = atan2(-gx, -gz) * 180 / PI;

  // Compute the PID output based on the angle and the IR Sharp sensor values
  double sharp_front = analogRead(SHARP_FRONT_PIN);
  double sharp_left = (analogRead(SHARP_LEFT_FRONT_PIN) + analogRead(SHARP_LEFT_BACK_PIN)) / 2.0;
  double sharp_right = (analogRead(SHARP_RIGHT_FRONT_PIN) + analogRead(SHARP_RIGHT_BACK_PIN)) / 2.0;
  double error = TARGET_ANGLE - angle;
  integral += error;
  derivative = error - last_error;
  double output = KP * error + KI * integral + KD * derivative;
  last_error = error;

  // Adjust the motor speeds based on the PID output and IR Sharp sensor values
  int motor1_speed = PWM_VALUE + output + (sharp_left - sharp_right);
  int motor2_speed = PWM_VALUE - output - (sharp_left - sharp_right);
  analogWrite(MOTOR1_PIN, motor1_speed);
  analogWrite(MOTOR2_PIN, motor2_speed);

  // Print the angle, IR Sharp sensor values, and PID output for debugging
  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(", Front: ");
  Serial.print(sharp_front);
  Serial.print(", Left: ");
  Serial.print(sharp_left);
  Serial.print(", Right: ");
  Serial.print(sharp_right);
  Serial.print(", Output: ");
  Serial.println(output);

  delay(100);
}
