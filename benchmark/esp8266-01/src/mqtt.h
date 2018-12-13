#ifndef SRC_MQTT_H
#define SRC_MQTT_H

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "macro.hh"

// MQTT Server credentials
#ifdef MQTT_HOST
    const char* MQTT_SERVER_HOST = DEF_2_STR(MQTT_HOST);
#endif
#define MQTT_SERVER_PORT 8883
#ifdef MQTT_USER
    const char* MQTT_SERVER_USER = DEF_2_STR(MQTT_USER);
#endif
#ifdef MQTT_PASS
    const char* MQTT_SERVER_PASSWORD = DEF_2_STR(MQTT_PASS);
#endif

// Feeds
#define SEQUENCE_FEED "anxolerd/feeds/mqtt-benchmark-sequence"
#define COMMANDS_FEED "anxolerd/feeds/mqtt-benchmark-commands"

// Time to process incoming packets;
#define PROCESSING_TIME 4000

// Results
#define MEASUREMENTS 20
static long benchmarks[MEASUREMENTS];
static char receieved[MEASUREMENTS];

// STATUSES
#define STATUS_NOT_SENT 0
#define STATUS_SENT 1
#define STATUS_SEND_FAILED 2
#define STATUS_RECEIVED 4

void MQTT_connect(Adafruit_MQTT_Client);
void command_callback(uint32_t);
bool publish_sequence(Adafruit_MQTT_Publish, uint32_t);
bool check_all_arived();
void compute_stats();

#endif