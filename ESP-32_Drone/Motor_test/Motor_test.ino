const int motorPin = 12; // GPIO pin connected to MOSFET gate
const int pwmFrequency = 8000; // 5 kHz PWM frequency
const int pwmResolution = 8; // 8-bit resolution (0-255)

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200); // Initialize serial communication
  pinMode(motorPin, OUTPUT);

  // Configure LEDC PWM functionalities with the new API
  ledcAttach(motorPin, pwmFrequency, pwmResolution);
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

void loop() {
  // Gradually increase motor speed
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

  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(motorPin, dutyCycle);
    Serial.print("Increasing speed: ");
    Serial.println(dutyCycle);
    delay(100); // Adjust delay for smoother acceleration
  }
  delay(2000); // Run at full speed for 2 seconds

  // Gradually decrease motor speed
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(motorPin, dutyCycle);
    Serial.print("Decreasing speed: ");
    Serial.println(dutyCycle);
    delay(100); // Adjust delay for smoother deceleration
  }
  delay(2000); // Pause for 2 seconds

}
