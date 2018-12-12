#include <Arduino.h>

// Connectivity
#include "ESP8266WiFi.h"
#include "Credentials.hh"
#include "MQTT.hh"

// Sensors
#include "DHT.h"
#include "MQ8.hh"

// ********* Function defs *********
void processTemperatureAndHumidity(DHT&);
void processHydrogenGas(MQ8&);
// *********************************

// ********** Define pins **********
static const uint8_t DHT11_PIN = D7;
static const uint8_t MQ8_AOUT_PIN = A0;
static const uint8_t MQ8_DOUT_PIN = D2;
// *********************************

// ********* Define consts *********
const bool isFarengheit = false;
// *********************************


DHT dht(DHT11_PIN, DHT11);
MQ8 mq8(MQ8_AOUT_PIN, MQ8_DOUT_PIN);
MQTTClient mqtt(MqttServer);

void setup() {
  Serial.begin(9600);
  delay(100);

  Serial.printf("Initializing air quality sensor node ...\n");

  // Connect DHT11  
  Serial.print("- DHT11 sensor initialization ... ");
  dht.begin();
  Serial.println("OK");

  // Connect MQ8 Sensor
  Serial.print("- MQ8 Hydrogen Gas Sensor initialization ... ");
  mq8.begin();
  Serial.println("OK");

  // Connect to MQTT
  Serial.print(" - MQTT broker initialization ... ");
  mqtt.begin(WiFiSsid, WiFiPass);

  Serial.printf("---------- Initialization complete ----------\n");
}

void loop() {
  processTemperatureAndHumidity(dht);
  processHydrogenGas(mq8);
  delay(5000);
}


void processTemperatureAndHumidity(DHT& dht) {
  float temperature = dht.readTemperature(isFarengheit);
  float humidity = dht.readHumidity();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }  
  float heatIndex = dht.computeHeatIndex(temperature, humidity, isFarengheit);
  Serial.printf(
    "Temperature is %.2f°C, humidity is %.2f%%, heat index is %.2f.\n",
    temperature, humidity, heatIndex
  );
  mqtt.publishTemperature(temperature);
  mqtt.publishHumidity(humidity);
}

void processHydrogenGas(MQ8& mq8) {
  int ppm= mq8.readValue();
  bool isLimitReached = mq8.isLimitReached();
  Serial.printf(
    "Hydrogen value: %d ppm. %s\n",
     ppm,
     isLimitReached ? "Limit reached!" : "All fine."
  );
  mqtt.publishHydrogen(ppm);
}