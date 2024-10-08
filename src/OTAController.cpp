/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.4.0
 * 
*/

#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "OTAController.h"

void OTAController::initOTA(String hostname){

  ArduinoOTA.setHostname(hostname.c_str());
  ArduinoOTA.onStart([]() {});
  ArduinoOTA.onEnd([]() {});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});
  ArduinoOTA.onError([](ota_error_t error) {});
  ArduinoOTA.begin();
}

void OTAController::handleOTA(){
  ArduinoOTA.handle();
}
