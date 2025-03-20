#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define PUMP_PIN 34  // Potentiometer 1 (แทนตัวจับความชื้น)
#define RELAY_PIN 5

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "demo.thingsboard.io";
const char* ID = "14caa4c0-0551-11f0-a896-bbf2f9e9d0e5";
const char* token = "JXbA6r5F9UDreyQcIVSB";
const int port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 5000; 

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect(ID, token, "")) {
      Serial.println("Connected!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds...");
      delay(5000);
    }
  }
}

void sendTemperature(float temp) {
  String payload = "{\"temperature\": " + String(temp, 1) + "}"; 
  client.publish("v1/devices/me/telemetry", payload.c_str());
  Serial.println("Sent to ThingsBoard: " + payload);
}

void setup() {
  Serial.begin(115200);
  
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");

  client.setServer(mqtt_server, port);

  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int pump = analogRead(PUMP_PIN);
  Serial.print("HUM Level: ");
  Serial.println(pump);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float temp = dht.readTemperature();
    
    if (!isnan(temp)) {
      Serial.print("Temperature: ");
      Serial.println(temp);
      sendTemperature(temp);
    } else {
      Serial.println("Failed to read from DHT22!");
    }
  }

  if (pump > 2000) {  
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("ON");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("OFF");
  }

  delay(1000);
}
