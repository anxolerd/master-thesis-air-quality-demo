#ifndef MQTT_HH
#define MQTT_HH

#include "Arduino.h"
#include "ESP8266WiFi.h"

#include "PubSubClient.h"

class MQTTClient {
    public:
        MQTTClient(const char* mqttServer, uint16_t mqttPort=1883);
        void begin(const char* ssid, const char* pass);
        void publishTemperature(float temperature);
        void publishHumidity(float humidity);
        void publishHydrogen(int ppm);
        const char* localAddress();
        void reconnect();

        static const char* TEMPERATURE_TOPIC;
        static const char* HUMIDITY_TOPIC;
        static const char* HYDROGEN_TOPIC;
    private:
        WiFiClient _wifiClient;
        PubSubClient _client;
};

#endif