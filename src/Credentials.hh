#ifndef CREDENTIALS_HH
#define CREDENTIALS_HH

#include "macro.hh"

#ifdef WIFI_SSID
    const char* WiFiSsid = DEF_2_STR(WIFI_SSID);
#endif

#ifdef WIFI_PASS
    const char* WiFiPass = DEF_2_STR(WIFI_PASS);
#endif

#ifdef MQTT_SERVER
    const char* MqttServer = DEF_2_STR(MQTT_SERVER);
#endif

#endif