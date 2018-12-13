#include <Arduino.h>
#include "mqtt.h"

bool try_connect(Adafruit_MQTT_Client mqtt) {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected() && mqtt.ping()) {
    return true;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         Serial.println("Failed to connect to MQTT");
         return false;
       }
  }

  Serial.println("MQTT Connected!");
  return true;
}


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
char MQTT_connect(Adafruit_MQTT_Client mqtt, Adafruit_MQTT_Client backup) {
  Serial.println("Trying main...");
  if (try_connect(mqtt)) { return MQTT_MAIN; }
  Serial.println("Trying backup...");
  if (try_connect(backup)) { return MQTT_BACKUP; }
  return 0;
}

bool publish_sequence(Adafruit_MQTT_Publish publisher, uint32_t value) {
    benchmarks[value] = micros();
    int tries = 3;
    do {
        bool success = publisher.publish(value);
        if (success) { 
          Serial.printf("Published value %d\n", value);
          receieved[value] = STATUS_SENT;
          // Serial.printf("received[%d] = %d\n", value, receieved[value]);
          // Serial.printf("benchmarks[%d] = %d\n", value, benchmarks[value]);

          return true; 
        }
        tries--;
    } while (tries > 0);
    Serial.printf("Failed to publish value %d\n", value);
    receieved[value] = STATUS_SEND_FAILED;
    return false;
}

void command_callback(uint32_t command) {
    if (receieved[command] == STATUS_RECEIVED) {
      return;
    }
    benchmarks[command] = micros() - benchmarks[command];
    receieved[command] = STATUS_RECEIVED;
    Serial.printf("Received command: %d\n", command);
    // Serial.printf("received[%d] = %d\n", command, receieved[command]);
    // Serial.printf("benchmarks[%d] = %d\n", command, benchmarks[command]);
}

bool check_all_arived() {
  for (int i = 0; i < MEASUREMENTS; i++) {
    if (receieved[i] == STATUS_SENT) {
      return false;
    }
  }
  return true;
}

void compute_stats() {
  int successfull_messages = 0;
  unsigned long total_wait = 0;
  double average_time = 0;

  Serial.println("|-----------------|-------------");
  for (int i = 0; i < MEASUREMENTS; i++) {
    if (receieved[i] == STATUS_RECEIVED) {
      successfull_messages ++;
      total_wait += benchmarks[i];
      Serial.printf ("| Measurement %4d | %ld us \n", i, benchmarks[i]);
      Serial.println("|-----------------|-------------");
    }
  }

  Serial.println("|-----------------|-------------");
  Serial.println("|-----------------|-------------");
  average_time = (long double) total_wait / (double) successfull_messages;

  Serial.printf("| Successful      | %d    \n", successfull_messages);
  Serial.printf("| Total wait      | %ld us \n", total_wait);
  Serial.printf("| Average delay   | %lf us \n", average_time);
  Serial.println("|-----------------|------------|");
}