/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.4.0
 * 
*/
 
#ifndef Framework_h
#define Framework_h


#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.hpp>

//Include internal Structures

// Include internal Controllers
#include "EepromController.h"
#include "WifiController.h"
#include "MdnsController.h"
#include "OTAController.h"

// Optional Settings
const String DEVICE_NAME = "Bedroom-RollerBlind";
const int WIFI_CON_WAIT = 20;

class Framework{
    public:
        Framework(AsyncWebServer& server);
        void setup();
        void loop();
        AsyncWebServer& _server;
        //AsyncWebServer& getServer();
    private:
       
        void wifiSelect(AsyncWebServerRequest *request);
        void setWifiCreds(AsyncWebServerRequest *request);
        void notConnectedRoutes();
        void setupCredsRoutine();
        void landing(AsyncWebServerRequest *request);
        void clearEepromFull(AsyncWebServerRequest *request);
        void clearEepromAdminPass(AsyncWebServerRequest *request);
        void setAdminPass(AsyncWebServerRequest *request);
        void adminLogin(AsyncWebServerRequest *request);
        void dashboard(AsyncWebServerRequest *request);
        void connectedRoutes();
        void startEeprom();
};

#endif
