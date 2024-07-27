#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Redmi Note 10 Pro";
const char* password = "jrs12345";

WebServer server(80);

const int motorPin1 = 12; // Motor control pin
const int motorPin2 = 26; // Motor control pin
const int motorPin3 = 27; // Motor control pin
const int motorPin4 = 32; // Motor control pin

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Drone Control</h1>";
  html += "<form action=\"/control\" method=\"get\">";
  html += "Motor1 Speed: <input type=\"range\" name=\"motor1\" min=\"0\" max=\"255\" value=\"0\"><br>";
  html += "Motor2 Speed: <input type=\"range\" name=\"motor2\" min=\"0\" max=\"255\" value=\"0\"><br>";
  html += "Motor3 Speed: <input type=\"range\" name=\"motor3\" min=\"0\" max=\"255\" value=\"0\"><br>";
  html += "Motor4 Speed: <input type=\"range\" name=\"motor4\" min=\"0\" max=\"255\" value=\"0\"><br>";
  html += "<input type=\"submit\" value=\"Set Speed\">";
  html += "</form></body></html>";
  server.send(200, "text/html", html);
}

void handleControl() {
  if (server.hasArg("motor1")) {
    int speed1 = server.arg("motor1").toInt();
    ledcWrite(motorPin1, speed1);
    Serial.print("Motor1 Speed: ");
    Serial.println(speed1);
  }
  if (server.hasArg("motor2")) {
    int speed2 = server.arg("motor2").toInt();
    ledcWrite(motorPin2, speed2);
    Serial.print("Motor2 Speed: ");
    Serial.println(speed2);
  }
  if (server.hasArg("motor3")) {
    int speed3 = server.arg("motor3").toInt();
    ledcWrite(motorPin3, speed3);
    Serial.print("Motor3 Speed: ");
    Serial.println(speed3);
  }
  if (server.hasArg("motor4")) {
    int speed4 = server.arg("motor4").toInt();
    ledcWrite(motorPin4, speed4);
    Serial.print("Motor4 Speed: ");
    Serial.println(speed4);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/control", HTTP_GET, handleControl);
  server.begin();

  ledcAttach(motorPin1, 8000, 8); // Attach PWM channel 0 to motorPin1
  ledcAttach(motorPin2, 8000, 8); // Attach PWM channel 1 to motorPin2
  ledcAttach(motorPin3, 8000, 8); // Attach PWM channel 2 to motorPin3
  ledcAttach(motorPin4, 8000, 8); // Attach PWM channel 3 to motorPin4
}

void loop() {
  server.handleClient();
}
