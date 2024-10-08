// /*
//  * Author Alex Waine
//  * 
//  * Esp8266 Setup Firmware  
//  * 
//  * Version 1.4.0
//  * 
// */
#include "Framework.h"
#include <ESPAsyncWebServer.h>


AsyncWebServer server(80); 
Framework framework(server);

/*
 * Main Setup
 */
void setup() {
  framework.setup();

  framework._server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send( 200, "text/plain", "test");
  });

}

/*
 * Main Loop
 */
void loop() {
  
  framework.loop();

}
