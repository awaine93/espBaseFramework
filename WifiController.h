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

#ifdef ESP32
  #include <WiFi.h>
#endif

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif

class WifiController{
public:
   String getSsidOptions();
   void setupWifiAp(String);
   void forgetWifi();
   bool wifiConnTimer(int, String, String, String);
};

#endif
