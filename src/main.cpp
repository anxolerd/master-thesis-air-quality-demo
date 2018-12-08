#include <Arduino.h>

#include "DHT.h"
#include "MQ8.hh"

// ********* Function defs *********
void processTemperatureAndHumidity(DHT&);
void processHydrogenGas(MQ8&);
// *********************************

// ********** Define pins **********
static const uint8_t DHT11_PIN = D7;
static const uint8_t MQ8_AOUT_PIN = A0;
static const uint8_t MQ8_DOUT_PIN = D8;
// *********************************

// ********* Define consts *********
const bool isFarengheit = false;
// *********************************


DHT dht(DHT11_PIN, DHT11);
MQ8 mq8(MQ8_AOUT_PIN, MQ8_DOUT_PIN);

void setup() {
  Serial.begin(9600);

  Serial.printf("Initializing air quality sensor ...\n");
  
  dht.begin();
  Serial.printf("(+) DHT11 sensor initialized ...\n");

  mq8.begin();
  Serial.printf("(+) MQ8 Hydrogen Gas Sensor initialized ...\n");

  Serial.printf("---------- Initialization complete ----------\n");
}

void loop() {
  processTemperatureAndHumidity(dht);
  processHydrogenGas(mq8);
  delay(1000);
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
    "Temperature is %fC, humidity is %f %%, HeatIndex is %f\n",
    temperature, humidity, heatIndex
  );
}

void processHydrogenGas(MQ8& mq8) {
  int value= mq8.readValue(); //reads the analaog value from the hydrogen sensor's AOUT pin
  bool isLimitReached = mq8.isLimitReached();//reads the digital value from the hydrogen sensor's DOUT pin
  Serial.print("Hydrogen value: ");
  Serial.print(value);//prints the hydrogen value
  Serial.print(" Limit reached?: ");
  Serial.println(isLimitReached);
}