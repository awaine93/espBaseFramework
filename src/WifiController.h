#ifndef WifiController_h
#define WifiController_h

#ifdef ESP32
    #include <WiFi.h>
#else
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
