#ifndef MQTT_HH
#define MQTT_HH

#include "Arduino.h"
#include "stdlib.h"
#include "ESP8266WiFi.h"

#include "PubSubClient.h"

class MQTTClient {
    public:
        MQTTClient(const char* mqttServer, uint16_t mqttPort=1883);
        void begin(const char* ssid, const char* pass);
        const char* localAddress();
        void reconnect();
        bool isConnected();

        void subscribeTemperature(std::function<void(uint8_t*, unsigned int)>);
        void subscribeHumidity(std::function<void(uint8_t*, unsigned int)>);
        void subscribeHydrogen(std::function<void(uint8_t*, unsigned int)>);
        void subscribeCommand(std::function<void(uint8_t*, unsigned int)>);
        void loop();

        static const char* TEMPERATURE_TOPIC;
        static const char* HUMIDITY_TOPIC;
        static const char* HYDROGEN_TOPIC;
        static const char* COMMAND_TOPIC;
    private:
        WiFiClient _wifiClient;
        PubSubClient _client;
        std::function<void(uint8_t*, unsigned int)> _temperatureCb;
        std::function<void(uint8_t*, unsigned int)> _humidityCb;
        std::function<void(uint8_t*, unsigned int)> _hydrogenCb;
        std::function<void(uint8_t*, unsigned int)> _commandCb;

        void _callback(char* topic, uint8_t* payload, unsigned int length);

};

#endif