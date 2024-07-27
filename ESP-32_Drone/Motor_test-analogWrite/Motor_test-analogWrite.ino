const int ledPin = 2;  // 16 corresponds to GPIO 16

void setup() {
  // set the LED as an output
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop(){
  // increase the LED brightness
  for(int dutyCycle = 100; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    Serial.print("Increasing speed: ");
    Serial.println(dutyCycle);
    delay(100);
  }
  delay(5000);

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 100; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    Serial.print("decreasingspeed: ");
    Serial.println(dutyCycle);
    delay(100);
  }
    delay(5000);
}