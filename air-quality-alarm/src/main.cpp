#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <Servo.h>

#include "RGBLed.hh"
#include "Credentials.hh"
#include "MQTT.hh"

const long SLEEP_TIME = 1000;

// ********** Define pins **********
const uint8_t LED_RGB_R_PIN = D2;
const uint8_t LED_RGB_G_PIN = D3;
const uint8_t LED_RGB_B_PIN = D4;

const uint8_t LED_PIN = D6;

const uint8_t SERVO_PIN = D9;
// *********************************

// ********* Function defs *********
void onTemperatureReceived(uint8_t* payload, unsigned int length);
void onHumidityReceived(uint8_t* payload, unsigned int length);
void onHydrogenReceived(uint8_t* payload, unsigned int length);
void onCommandReceived(uint8_t* payload, unsigned int length);
// *********************************

// *********** Constants **********
const char* ENABLE_AIR_COND = "enableAirCondition";
const char* DISABLE_AIR_COND = "disableAirCondition";
// ********************************


MQTTClient mqtt(MqttServer);
RGBLed rgb(LED_RGB_R_PIN, LED_RGB_G_PIN, LED_RGB_B_PIN);
Servo servo;

float temperature = 0.0;
float humidity = 0.0;
int hydrogen = 0;

void setup() {
  Serial.begin(9600);
  delay(100);

  Serial.print("- Initialize RGB LED ... ");
  rgb.begin();
  Serial.println("OK");

  Serial.print("- Initialize LED ... ");
  pinMode(LED_PIN, OUTPUT);
  Serial.println("OK");

  Serial.print("- Initialize servo ... ");
  servo.attach(SERVO_PIN);
  servo.write(90);
  Serial.println("OK");

  Serial.print("- Initialize MQTT connection ...");
  mqtt.begin(WiFiSsid, WiFiPass);
  mqtt.subscribeTemperature(onTemperatureReceived);
  mqtt.subscribeHumidity(onHumidityReceived);
  mqtt.subscribeHydrogen(onHydrogenReceived);
  mqtt.subscribeCommand(onCommandReceived);

  Serial.println("----------- Initialization complete ----------");
}

void loop() {
  Serial.printf("Temperature: %.2fC, Humidity: %.2f%%, Hydrogen: %d ppm\n", temperature, humidity, hydrogen);
  mqtt.loop();

  if (temperature < 16.0) {
    rgb.blue();
  } else if (temperature < 24.0) {
    rgb.none();
  } else {
    rgb.red();
  }

  char color = rgb.getColor();
  if (humidity < 35) {
    rgb.yellow();
    delay(100);
    rgb.color(color);
  } else if (humidity > 60) {
    rgb.white();
    delay(100);
    rgb.color(color);
  }

  if (hydrogen > 900) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(1000);
}

void onTemperatureReceived(uint8_t* payload, unsigned int length) {
  char* buffer = new char[length+1];
  memcpy(buffer, payload, length);
  buffer[length] = '\0';
  temperature  = String(buffer).toFloat();
  delete[] buffer;
}

void onHumidityReceived(uint8_t* payload, unsigned int length) {
  char* buffer = new char[length+1];
  memcpy(buffer, payload, length);
  buffer[length] = '\0';

  humidity = String(buffer).toFloat();
  delete[] buffer;
}

void onHydrogenReceived(uint8_t* payload, unsigned int length) {
  char* buffer = new char[length+1];
  memcpy(buffer, payload, length);
  buffer[length] = '\0';

  hydrogen = String(buffer).toFloat();
  delete[] buffer;
}

void onCommandReceived(uint8_t* payload, unsigned int length) {
  char* command = new char[length+1];
  memcpy(command, payload, length);
  command[length] = '\0';

  Serial.printf("Command received: %s\n", command);
  if (!strcmp(command, ENABLE_AIR_COND)) {
    servo.write(7);
    return;
  }
  if (!strcmp(command, DISABLE_AIR_COND)) {
    servo.write(180);
    return;
  }
}