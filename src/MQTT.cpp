#include "MQTT.hh"

const char* MQTTClient::TEMPERATURE_TOPIC = "anxolerd/feeds/temperature";
const char* MQTTClient::HUMIDITY_TOPIC="anxolerd/feeds/humidity";
const char* MQTTClient::HYDROGEN_TOPIC="anxolerd/feeds/hydrogen";


MQTTClient::MQTTClient(const char* mqttServer, uint16_t mqttPort) {
    _client.setClient(_wifiClient);
    _client.setServer(mqttServer, mqttPort);
}

void MQTTClient::begin(const char* ssid, const char* pass) {
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("OK");
    Serial.printf("  IP address: %s\n", localAddress());
    reconnect();
}

void MQTTClient::reconnect() {
  // Loop until we're reconnected
  while (!_client.connected()) {
    Serial.print("  Attempting MQTT connection ... ");

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (_client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

const char* MQTTClient::localAddress() {
    return WiFi.localIP().toString().c_str();
}


void MQTTClient::publishTemperature(float temperature) {
    reconnect();
    _client.publish(TEMPERATURE_TOPIC, String(temperature).c_str());
}

void MQTTClient::publishHumidity(float humidity) {
    reconnect();
    _client.publish(HUMIDITY_TOPIC, String(humidity).c_str());
}

void MQTTClient::publishHydrogen(int ppm) {
    reconnect();
    _client.publish(HYDROGEN_TOPIC, String(ppm).c_str());
}
