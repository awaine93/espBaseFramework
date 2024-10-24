#ifndef EspBaseFramework_h
#define EspBaseFramework_h

#ifdef ESP32
    #include <WiFi.h>
    #include <AsyncTCP.h>
#else
    #include <ESP8266WiFi.h>
    #include <ESPAsyncTCP.h>
#endif


#include <DNSServer.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <ESPAsyncWebServer.h>


// Include internal Controllers
#include "EepromController.h"
#include "WifiController.h"
#include "MdnsController.h"
#include "OTAController.h"

// Project Specific Settings
extern const String DEVICE_NAME;
extern const int WIFI_CON_WAIT;
extern const String loggedInRoute;

class Framework{
    public:
        Framework(AsyncWebServer& server);
        void begin();
        void run();
        void testFs(AsyncWebServerRequest *request);
        AsyncWebServer& _server;
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
