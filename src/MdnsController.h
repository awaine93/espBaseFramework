#ifndef MdnsController_h
#define MdnsController_h

#ifdef ESP32
    #include <ESPmDNS.h>
    #include <WiFi.h>
#else
    #include <ESP8266mDNS.h>
    #include <ESP8266WiFi.h>
#endif




class MdnsController{
public:
   void loopHandle();
   void initMdns(String);
};

#endif
