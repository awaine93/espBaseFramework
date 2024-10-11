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

#include <ESP8266WiFi.h>

class MdnsController{
public:
   void loopHandle();
   void initMdns(String);
};

#endif
