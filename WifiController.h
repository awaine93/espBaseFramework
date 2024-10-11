#ifndef WifiController_h
#define WifiController_h

#include <ESP8266WiFi.h>

class WifiController{
public:
   String getSsidOptions();
   void setupWifiAp(String);
   void forgetWifi();
   void wifiConnTimer(int, String, String, String);
};

#endif
