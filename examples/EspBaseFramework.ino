#include "EspBaseFramework.h"
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// Uncomment for ESP32
//#define ESP32

const String DEVICE_NAME = "CHANGE-ME";
const String loggedInRoute = "/dashboard";
const int WIFI_CON_WAIT = 20;

AsyncWebServer server(80); 
Framework framework(server);

/*
 * Main Setup
 */
void setup() {
  framework.begin();

  framework._server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request){
      if(!LittleFS.exists("/DashboardPage.html")) {
          request->send(404, "text/html", "Page not found");
      }

      request->send(LittleFS, "/DashboardPage.html", "text/html");
  });
}

/*
 * Main Loop
 */
void loop() {
  
  framework.run();

}
