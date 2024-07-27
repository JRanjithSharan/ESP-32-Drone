#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// Motor pins
const int motorPin1 = 12;  // CW
const int motorPin2 = 26;  // CCW
const int motorPin3 = 27;  // CW
const int motorPin4 = 32;  // CCW

const int pwmFrequency = 8000; // 8 kHz PWM frequency
const int pwmResolution = 8;   // 8-bit resolution (0-255)

Adafruit_MPU6050 mpu;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

WebServer server(80);

float pidRoll, pidPitch, pidYaw;
float pidRollSetpoint = 0, pidPitchSetpoint = 0, pidYawSetpoint = 0;
float lastRollError, lastPitchError, lastYawError;
float integralRoll, integralPitch, integralYaw;

float kp = 1.0; // Proportional gain
float ki = 0.05; // Integral gain
float kd = 0.01; // Derivative gain

unsigned long lastTime;

void setup() {
  Serial.begin(115200);
  
  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Initialize HMC5883
  if (!mag.begin()) {
    Serial.println("Failed to find HMC5883 chip");
    while (1) { delay(10); }
  }

  // Configure LEDC PWM functionalities and attach pins
  ledcAttach(motorPin1, pwmFrequency, pwmResolution);
  ledcAttach(motorPin2, pwmFrequency, pwmResolution);
  ledcAttach(motorPin3, pwmFrequency, pwmResolution);
  ledcAttach(motorPin4, pwmFrequency, pwmResolution);

  // Initialize WiFi
  WiFi.begin("Redmi Note 10 Pro", "jrs12345");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/plain", "Drone connected!");
  });

  server.on("/control", HTTP_POST, []() {
    String body = server.arg("plain");
    StaticJsonDocument<200> doc;
    deserializeJson(doc, body);
    pidRollSetpoint = doc["roll"];
    pidPitchSetpoint = doc["pitch"];
    pidYawSetpoint = doc["yaw"];
    server.send(200, "text/plain", "Control values set!");
  });

  server.begin();

  lastTime = millis();
}

void loop() {
  server.handleClient();

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  sensors_event_t event;
  mag.getEvent(&event);

  unsigned long currentTime = millis();
  float elapsedTime = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  float roll = a.acceleration.x;
  float pitch = a.acceleration.y;
  float yaw = g.gyro.z;
  float heading = atan2(event.magnetic.y, event.magnetic.x);

  // Convert heading to degrees
  heading = heading * 180 / PI;

  // PID calculations
  pidRoll = calculatePID(pidRollSetpoint, roll, lastRollError, integralRoll, elapsedTime);
  pidPitch = calculatePID(pidPitchSetpoint, pitch, lastPitchError, integralPitch, elapsedTime);
  pidYaw = calculatePID(pidYawSetpoint, heading, lastYawError, integralYaw, elapsedTime);

  // Motor speed adjustments
  int motorSpeed1 = constrain(128 + pidRoll - pidPitch + pidYaw, 0, 255);
  int motorSpeed2 = constrain(128 - pidRoll - pidPitch - pidYaw, 0, 255);
  int motorSpeed3 = constrain(128 + pidRoll + pidPitch - pidYaw, 0, 255);
  int motorSpeed4 = constrain(128 - pidRoll + pidPitch + pidYaw, 0, 255);

  ledcWrite(0, motorSpeed1);
  ledcWrite(1, motorSpeed2);
  ledcWrite(2, motorSpeed3);
  ledcWrite(3, motorSpeed4);
/*
  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print(" Pitch: ");
  Serial.print(pitch);
  Serial.print(" Yaw: ");
  Serial.print(yaw);
  Serial.print(" Heading: ");
  Serial.println(heading);
*/
}

float calculatePID(float setpoint, float current, float &lastError, float &integral, float dt) {
  float error = setpoint - current;
  integral += error * dt;
  float derivative = (error - lastError) / dt;
  lastError = error;
  return (kp * error) + (ki * integral) + (kd * derivative);
}
