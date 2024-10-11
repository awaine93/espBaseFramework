/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.1.0
 * 
*/

#include "MdnsController.h" 
#include <ESP8266mDNS.h>

void MdnsController::loopHandle(){
    MDNS.update();
}

void MdnsController::initMdns(String deviceName){
  
   if (!MDNS.begin(deviceName)) {
    Serial.println("Error setting up MDNS responder!");
  }

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
  //Serial.println("mDNS responder started");
}
