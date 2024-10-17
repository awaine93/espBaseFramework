/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.1.0
 * 
*/

#ifndef MdnsController_h
#define MdnsController_h

#ifdef ESP32
  #include <WiFi.h>
  #include <ESPmDNS.h>
#endif

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>

#endif

class MdnsController{
public:
//   void loopHandle();
   void initMdns(String);
};

#endif
