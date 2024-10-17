/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.1.0
 * 
*/

#ifndef EepromController_h
#define EepromController_h

#ifdef ESP32
  #include <WiFi.h>
#endif

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif

class EepromController{
public:
   void startEeprom();
   void wipe();
   void clearAdminPass();
   void storeWifiCreds(String,String);
   void storeAdminPass(String);
   String eepromGetWifiSsid();
   String eepromGetWifiPass();
   String isAdminPassSet();
   String getAdminPass();
};

#endif
