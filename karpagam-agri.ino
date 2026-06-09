#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
//https://esskay-012024.live/plant/robo_menu.php
// ---------------- MOTOR ----------------
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14

// ---------------- SENSOR ----------------
#define DHTPIN 4
#define DHTTYPE DHT11  
#define SOIL_PIN 34

// ---------------- RELAY ----------------
#define RELAY1 16
#define RELAY2 17

DHT dht(DHTPIN, DHTTYPE);

// ---------------- WIFI ----------------
const char* ssid = "Firmware";
const char* password = "Solutions@12345";

// ---------------- SERVER URLs ----------------
String motorURL = "http://esskay-012024.live/plant/get_command.php";
String relayURL = "http://esskay-012024.live/plant/get_relay_status.php";

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Relay pins
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  readSensors();
  fetchMotorCommand();
  fetchRelayStatus();
  delay(2000);

  

}

// ================= SENSOR =================

void readSensors() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int soilValue = analogRead(SOIL_PIN);

  Serial.printf("Temp: %.2f | Humidity: %.2f | Soil: %d\n",
                temperature, humidity, soilValue);

  sendSensorToServer(temperature, humidity, soilValue);

  if (soilValue > 1000) {
    stop1();
    Serial.println("Motor1 on");
    delay(2000);
    digitalWrite(RELAY1, HIGH);
    delay(2000);
    digitalWrite(RELAY1, LOW);
    delay(2000);
  }
}


// ================= MOTOR FETCH =================

void fetchMotorCommand() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(motorURL);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String command = http.getString();
    command.trim();
    Serial.println("Motor Command: " + command);

    if (command == "forward") forward();
    else if (command == "reverse") reverse();
    else if (command == "left") left();
    else if (command == "right") right();
    else stop1();
  }
  http.end();
}

// ================= RELAY FETCH =================

void fetchRelayStatus() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(relayURL);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String status = http.getString();
    status.trim();
    Serial.println("Relay Status: " + status);

    if (status == "ON" || status == "1") {
      Serial.println("Motor2 on");
      stop1();delay(1000);
      digitalWrite(RELAY2, HIGH);
      Serial.println("Relay ON");
      delay(5000);
     digitalWrite(RELAY2, LOW);
      
    } else {
      digitalWrite(RELAY2, LOW);
      Serial.println("Relay OFF");
    }
  }
  http.end();
}

// ================= MOTOR FUNCTIONS =================

void reverse() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void forward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  Serial.println("Reverse");
}

void left() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  Serial.println("Left");
}

void right() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop1() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  Serial.println("Stop");
}


void sendSensorToServer(float temp, float hum, int soil) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;

  String url = "http://esskay-012024.live/plant/store_sensor.php";
  String postData = "temperature=" + String(temp) +
                    "&humidity=" + String(hum) +
                    "&soil=" + String(soil);

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.println("Server: " + response);
  } else {
    Serial.println("Error sending data");
  }

  http.end();
}
