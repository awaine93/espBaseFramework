#include "EspBaseFramework.h"
#include <ESPAsyncWebServer.h>
#include "Pages/DashboardPage.h"

// Uncomment for ESP32
//#define ESP32

const String dashboardPage = FPSTR(DASHBOARD_page);

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
      request->send( 200, "text/html", dashboardPage);
  });

}

/*
 * Main Loop
 */
void loop() {
  
  framework.run();

}
