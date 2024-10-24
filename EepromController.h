#ifndef EepromController_h
#define EepromController_h

#ifdef ESP32
    #include <WiFi.h>
#else
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
