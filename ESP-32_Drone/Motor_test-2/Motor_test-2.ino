const int motorPins[] = {12,26, 27, 32}; // GPIO pins connected to MOSFET gates for each motor
const int pwmFrequency = 8000; // 8 kHz PWM frequency
const int pwmResolution = 8; // 8-bit resolution (0-255)
const int pwmChannels[] = {0, 1, 2, 3}; // PWM channels for each motor

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200); // Initialize serial communication

  // Configure each motor pin as output and attach to a PWM channel
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
    ledcAttachChannel(motorPins[i], pwmFrequency, pwmResolution,pwmChannels[i]); // Attach pin to PWM with specified frequency and resolution

      Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for Serial to be ready
  }

  // Try to initialize the MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 Found!");

  // Configure the sensor
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
  }
}

void loop() {
  // Gradually increase motor speeds
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Accel X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Gyro X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temp: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");

  for (int dutyCycle = 65; dutyCycle <= 255; dutyCycle++) {
    for (int i = 0; i < 4 ; i++) {
      ledcWrite(motorPins[i], dutyCycle); // Write PWM value to each motor channel
      Serial.print("Increasing speed of motor ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(dutyCycle);
    }
    delay(200); // Adjust delay for smoother acceleration
  }
  delay(2000); // Run at full speed for 2 seconds

  // Gradually decrease motor speeds
  for (int dutyCycle = 255; dutyCycle >= 65; dutyCycle--) {
    for (int i = 0; i < 4; i++) {
      ledcWrite(motorPins[i], dutyCycle); // Write PWM value to each motor channel
      Serial.print("Decreasing speed of motor ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(dutyCycle);
    }
    delay(200); // Adjust delay for smoother deceleration
  }
  delay(5000); // Pause for 2 seconds
}
