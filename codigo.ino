#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"
#include <ArduinoJson.h>
 
const char* WIFI_SSID = "FIAP-IOT";
const char* WIFI_PASS = "F!@p25.IOT";
 
const char* MQTT_SERVER = "54.221.163.3";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_USER = "";
const char* MQTT_PASS = "";
 
const char* TOPIC_PUBLISH = "sensors/esp32/data";
const char* TOPIC_COMMAND = "sensors/esp32/command";
const char* TOPIC_STATUS  = "sensors/esp32/status";
 
const int DHT_PIN = 4;
const int LDR_PIN = 34;
const int BUZZER_PIN = 12;
 
const unsigned long PUBLISH_INTERVAL_MS = 30000;
const int ADC_MAX = 4095;
const float TEMP_ALERT_C = 30.0;
 
DHTesp dht;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
 
unsigned long lastPublish = 0;
unsigned long lastBuzzerToggle = 0;
bool buzzerState = false;
bool buzzerForced = false;
bool buzzerAutoAlarm = true;
unsigned long buzzerIntervalMs = 500;
 
void setBuzzerOnOff(bool on) {
  digitalWrite(BUZZER_PIN, on ? HIGH : LOW);
  buzzerState = on;
}
 
void handleCommand(const String &cmd) {
  if (cmd == "BUZZER_ON") {
    buzzerForced = true;
    buzzerAutoAlarm = false;
    setBuzzerOnOff(true);
    mqttClient.publish(TOPIC_STATUS, "BUZZER_ON");
  } else if (cmd == "BUZZER_OFF") {
    buzzerForced = false;
    buzzerAutoAlarm = false;
    setBuzzerOnOff(false);
    mqttClient.publish(TOPIC_STATUS, "BUZZER_OFF");
  } else if (cmd == "ALARM_ON") {
    buzzerAutoAlarm = true;
    mqttClient.publish(TOPIC_STATUS, "ALARM_ON");
  } else if (cmd == "ALARM_OFF") {
    buzzerAutoAlarm = false;
    setBuzzerOnOff(false);
    mqttClient.publish(TOPIC_STATUS, "ALARM_OFF");
  } else if (cmd.startsWith("SET_BUZZER_INTERVAL:")) {
    String val = cmd.substring(cmd.indexOf(':') + 1);
    long v = val.toInt();
    if (v > 50) {
      buzzerIntervalMs = v;
      mqttClient.publish(TOPIC_STATUS, "BUZZER_INTERVAL_SET");
    }
  } else if (cmd == "PING") {
    mqttClient.publish(TOPIC_STATUS, "PONG");
  } else if (cmd == "RESTART") {
    mqttClient.publish(TOPIC_STATUS, "RESTARTING");
    delay(500);
    ESP.restart();
  } else {
    mqttClient.publish(TOPIC_STATUS, "UNKNOWN_CMD");
  }
}
 
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  handleCommand(msg);
}
 
void connectMQTT() {
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  if (WiFi.status() != WL_CONNECTED) return;
  while (!mqttClient.connected()) {
    String clientId = "ESP32Client-";
    clientId += String((uint64_t)ESP.getEfuseMac(), HEX);
    bool connected;
    if (String(MQTT_USER) == "") {
      connected = mqttClient.connect(clientId.c_str());
    } else {
      connected = mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PASS);
    }
    if (connected) {
      mqttClient.subscribe(TOPIC_COMMAND);
      mqttClient.publish(TOPIC_STATUS, "ONLINE");
    } else {
      delay(3000);
    }
  }
}
 
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (millis() - start > 30000) return;
  }
}
 
void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  dht.setup(DHT_PIN, DHTesp::DHT11);
  analogReadResolution(12);
  connectWiFi();
  if (WiFi.status() == WL_CONNECTED) connectMQTT();
  lastPublish = millis();
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!mqttClient.connected()) connectMQTT();
    mqttClient.loop();
  } else {
    static unsigned long lastTry = 0;
    if (millis() - lastTry > 10000) {
      lastTry = millis();
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
  }
 
  unsigned long now = millis();
 
  if (now - lastPublish >= PUBLISH_INTERVAL_MS) {
    lastPublish = now;
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    int raw = analogRead(LDR_PIN);
    int lightPercent = map(raw, 0, ADC_MAX, 100, 0);
    if (lightPercent < 0) lightPercent = 0;
    if (lightPercent > 100) lightPercent = 100;
    StaticJsonDocument<256> doc;
    if (!isnan(temperature)) doc["temperature"] = temperature;
    else doc["temperature"] = nullptr;
    if (!isnan(humidity)) doc["humidity"] = humidity;
    else doc["humidity"] = nullptr;
    doc["light"] = raw;
    doc["light_percent"] = lightPercent;
    doc["ip"] = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : "no_wifi";
    char payload[256];
    serializeJson(doc, payload);
    if (mqttClient.connected()) mqttClient.publish(TOPIC_PUBLISH, payload);
    else Serial.println("MQTT not connected");
  }
 
  if (buzzerAutoAlarm && !buzzerForced) {
    float t = dht.getTemperature();
    if (!isnan(t) && t >= TEMP_ALERT_C) {
      if (millis() - lastBuzzerToggle >= buzzerIntervalMs) {
        lastBuzzerToggle = millis();
        buzzerState = !buzzerState;
        setBuzzerOnOff(buzzerState);
      }
    } else {
      if (buzzerState) {
        buzzerState = false;
        setBuzzerOnOff(false);
      }
    }
  }
 
  delay(10);
}
 
 
