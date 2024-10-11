// /*
//  * Author Alex Waine
//  * 
//  * Esp8266 Setup Firmware  
//  * 
//  * Version 1.1.0
//  * 
// */
#include "EspBaseFramework.h"
#include <ESPAsyncWebServer.h>
#include "Pages/DashboardPage.h"

const String dashboardPage = FPSTR(DASHBOARD_page);


AsyncWebServer server(80); 
Framework framework(server);

/*
 * Main Setup
 */
void setup() {
  framework.begin();

  framework._server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send( 200, "text/plain", dashboardPage);
  });

}

/*
 * Main Loop
 */
void loop() {
  
  framework.run();

}
