#ifndef MdnsController_h
#define MdnsController_h

#ifdef ESP32
    #include <ESPmDNS.h>
#else
    #include <ESP8266mDNS.h>
#endif


class MdnsController{
public:
   void loopHandle();
   void initMdns(String);
};

#endif
