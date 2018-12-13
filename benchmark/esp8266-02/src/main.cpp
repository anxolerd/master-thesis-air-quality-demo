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

WiFiClient client;
WiFiClientSecure secure_client;

Adafruit_MQTT_Client mqtt_main(
  &secure_client,
  MQTT_SERVER_HOST,
  MQTT_SERVER_PORT,
  MQTT_SERVER_USER,
  MQTT_SERVER_PASSWORD
);

Adafruit_MQTT_Client mqtt_backup(
  &client,
  MQTT_BACKUP_SERVER_HOST,
  MQTT_BACKUP_SERVER_PORT
);

Adafruit_MQTT_Subscribe commands_feed_main = Adafruit_MQTT_Subscribe(&mqtt_main, COMMANDS_FEED);
Adafruit_MQTT_Publish sequence_feed_main   = Adafruit_MQTT_Publish(&mqtt_main, SEQUENCE_FEED, MQTT_QOS_1);

Adafruit_MQTT_Subscribe commands_feed_backup = Adafruit_MQTT_Subscribe(&mqtt_backup, COMMANDS_FEED);
Adafruit_MQTT_Publish sequence_feed_backup   = Adafruit_MQTT_Publish(&mqtt_backup, SEQUENCE_FEED, MQTT_QOS_1);


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

  commands_feed_main.setCallback(command_callback);
  mqtt_main.subscribe(&commands_feed_main);
  commands_feed_backup.setCallback(command_callback);
  mqtt_backup.subscribe(&commands_feed_backup);
}

void loop() {
  if (computed) { return; }
  
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  Adafruit_MQTT_Client *mqtt = NULL;
  Adafruit_MQTT_Publish *publisher = NULL;
  switch (MQTT_connect(mqtt_main, mqtt_backup))  {
    case MQTT_MAIN:
      mqtt = &mqtt_main;
      publisher = &sequence_feed_main;
      break;
    case MQTT_BACKUP:
      mqtt = &mqtt_backup;
      publisher = &sequence_feed_backup;
      break;
    default:
      return;
  }

  
  if (current_measure < MEASUREMENTS) {
    publish_sequence(*publisher, current_measure++);
  }

  // process messages for PROCESSING_TIME
  mqtt->processPackets(PROCESSING_TIME);
  // keep the connection alive
  mqtt->ping();

  if (current_measure >= MEASUREMENTS) {
    if (check_all_arived()) {
      compute_stats();
      computed = true;
    }
  }
}