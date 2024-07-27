#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <driver/ledc.h>

Adafruit_MPU6050 mpu;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// ESC pins
const int escPin1 = 13;
const int escPin2 = 14;
const int escPin3 = 25;
const int escPin4 = 26;

// PWM properties
const int freq = 5000;
const int resolution = 8;

// Channels
const int channel1 = 0;
const int channel2 = 1;
const int channel3 = 2;
const int channel4 = 3;

// PID parameters
float Kp = 1.0; // Proportional gain
float Ki = 0.5; // Integral gain
float Kd = 0.1; // Derivative gain

float setpoint = 0.0; // Desired orientation
float integral = 0;
float last_error = 0;

void setup() {
  Serial.begin(115200);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  
  if (!mag.begin()) {
    Serial.println("Failed to find HMC5883 chip");
    while (1) {
      delay(10);
    }
  }
  
  Serial.println("MPU6050 and HMC5883 initialized");

  // Configure PWM
  ledcSetup(channel1, freq, resolution);
  ledcSetup(channel2, freq, resolution);
  ledcSetup(channel3, freq, resolution);
  ledcSetup(channel4, freq, resolution);

  // Attach the channels to the GPIO pins
  ledcAttachPin(escPin1, channel1);
  ledcAttachPin(escPin2, channel2);
  ledcAttachPin(escPin3, channel3);
  ledcAttachPin(escPin4, channel4);

  // Initialize ESCs with minimum signal (usually 1000 microseconds)
  ledcWrite(channel1, 0);
  ledcWrite(channel2, 0);
  ledcWrite(channel3, 0);
  ledcWrite(channel4, 0);
  
  delay(5000); // Give ESCs time to initialize
}

float calculatePID(float setpoint, float measured_value) {
  float error = setpoint - measured_value;
  integral += error;
  float derivative = error - last_error;
  float output = Kp * error + Ki * integral + Kd * derivative;
  last_error = error;
  return output;
}

void loop() {
  // Read sensor data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  // Assuming we're using the pitch (angle around X-axis) for control
  float measured_value = a.acceleration.x; // Replace with actual axis as needed
  
  // Calculate PID output
  float pid_output = calculatePID(setpoint, measured_value);
  
  // Convert PID output to motor speed adjustments
  int base_speed = 128; // Mid-range value for 8-bit PWM (0-255)
  int escValue1 = constrain(base_speed + pid_output, 0, 255);
  int escValue2 = constrain(base_speed - pid_output, 0, 255);
  int escValue3 = constrain(base_speed + pid_output, 0, 255);
  int escValue4 = constrain(base_speed - pid_output, 0, 255);
  
  // Apply motor speeds
  ledcWrite(channel1, escValue1);
  ledcWrite(channel2, escValue2);
  ledcWrite(channel3, escValue3);
  ledcWrite(channel4, escValue4);

  delay(10);
}
