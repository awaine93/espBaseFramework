#include "MdnsController.h" 

#ifdef ESP32
  #include <ESPmDNS.h>
#else
  #include <ESP8266mDNS.h>
#endif

void MdnsController::loopHandle(){
    #ifndef ESP32
      MDNS.update();
    #endif
}

void MdnsController::initMdns(String deviceName){
  
   if (!MDNS.begin(deviceName)) {
    #ifdef SERIAL_DEBUGGING
      Serial.println("Error setting up MDNS responder!");
    #endif
  }

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  #ifdef SERIAL_DEBUGGING
    Serial.println("mDNS responder started");
  #endif
}
