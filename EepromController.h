/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.4.0
 * 
*/

#ifndef EepromController_h
#define EepromController_h

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
