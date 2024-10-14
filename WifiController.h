/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.1.0
 * 
*/
 
#ifndef WifiController_h
#define WifiController_h

#include <WiFi.h>

class WifiController{
public:
   String getSsidOptions();
   void setupWifiAp(String);
   void forgetWifi();
   void wifiConnTimer(int, String, String, String);
};

#endif
