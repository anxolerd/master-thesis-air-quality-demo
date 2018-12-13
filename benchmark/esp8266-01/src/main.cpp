#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "mqtt.h"
#include "macro.hh"

/************************* WiFi Access Point *********************************/
#ifdef WIFI_SSID
  const char* WLAN_SSID = DEF_2_STR(WIFI_SSID);
#endif
#ifdef WIFI_PASS
  const char* WLAN_PASS = DEF_2_STR(WIFI_PASS);
#endif

WiFiClientSecure client;
Adafruit_MQTT_Client mqtt(
  &client,
  MQTT_SERVER_HOST,
  MQTT_SERVER_PORT,
  MQTT_SERVER_USER,
  MQTT_SERVER_PASSWORD
);

Adafruit_MQTT_Subscribe commands_feed = Adafruit_MQTT_Subscribe(&mqtt, COMMANDS_FEED);
Adafruit_MQTT_Publish sequence_feed   = Adafruit_MQTT_Publish(&mqtt, SEQUENCE_FEED, MQTT_QOS_1);

int current_measure = 0;
bool computed;

void setup();
void loop();

void setup() {
  Serial.begin(9600);
  computed = false;
  delay(10);

  Serial.print(F("\nMQTT_Benchmark"));

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(" WiFi connected."));

  commands_feed.setCallback(command_callback);
  mqtt.subscribe(&commands_feed);
}

void loop() {
  if (computed) { return; }
  
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect(mqtt);

  
  if (current_measure < MEASUREMENTS) {
    publish_sequence(sequence_feed, current_measure++);
  }

  // process messages for PROCESSING_TIME
  mqtt.processPackets(PROCESSING_TIME);
  // keep the connection alive
  mqtt.ping();

  if (current_measure >= MEASUREMENTS) {
    if (check_all_arived()) {
      compute_stats();
      computed = true;
    }
  }
}