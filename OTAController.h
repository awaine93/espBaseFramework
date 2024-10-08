/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.4.0
 * 
*/

#ifndef OTAController_h
#define OTAController_h

class OTAController{
public:
   void initOTA(String);
   void handleOTA();
};

#endif
