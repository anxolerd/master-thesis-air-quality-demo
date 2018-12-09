#include "MQTT.hh"

const char* MQTTClient::TEMPERATURE_TOPIC = "anxolerd/feeds/temperature";
const char* MQTTClient::HUMIDITY_TOPIC="anxolerd/feeds/humidity";
const char* MQTTClient::HYDROGEN_TOPIC="anxolerd/feeds/hydrogen";

void _noop(uint8_t* payload, unsigned int) {}

MQTTClient::MQTTClient(const char* mqttServer, uint16_t mqttPort) {
    _client.setClient(_wifiClient);
    _client.setServer(mqttServer, mqttPort);

    _temperatureCb = _noop;
    _humidityCb = _noop;
    _hydrogenCb = _noop;
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
    std::function<void(char*, uint8_t*, unsigned int)>  cb = std::bind(
        &MQTTClient::_callback, this, 
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3
    );
    _client.setCallback(cb);
}

void MQTTClient::loop() {
    reconnect();
    _client.loop();
}

void MQTTClient::reconnect() {
  // Loop until we're reconnected
  while (!_client.connected()) {
    Serial.print("  Attempting MQTT connection ... ");

    // Create a random client ID
    String clientId = "ESP8266Client-alarm-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (_client.connect(clientId.c_str())) {
      Serial.println("connected");

      _client.subscribe(TEMPERATURE_TOPIC);
      _client.subscribe(HUMIDITY_TOPIC);
      _client.subscribe(HYDROGEN_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool MQTTClient::isConnected() {
    return _client.connected();
}

const char* MQTTClient::localAddress() {
    return WiFi.localIP().toString().c_str();
}

void MQTTClient::_callback(char* topic, uint8_t* payload, unsigned int length) {
    if (!strcmp(topic, TEMPERATURE_TOPIC)) return _temperatureCb(payload, length);
    if (!strcmp(topic, HUMIDITY_TOPIC)) return _humidityCb(payload, length);
    if (!strcmp(topic, HYDROGEN_TOPIC)) return _hydrogenCb(payload, length);
}

void MQTTClient::subscribeTemperature(std::function<void(uint8_t*, unsigned int)> cb) {
    _temperatureCb = cb;
}

void MQTTClient::subscribeHumidity(std::function<void(uint8_t*, unsigned int)> cb) {
    _humidityCb = cb;
}

void MQTTClient::subscribeHydrogen(std::function<void(uint8_t*, unsigned int)> cb) {
    _hydrogenCb = cb;
}