#define SERIAL_DEBUGGING

#include "EspBaseFramework.h"

// Internal variables
const byte DNS_PORT = 53;
String wifiOptions;
IPAddress dns_IP(192, 168, 1, 1);

// External Class instances 
DNSServer dnsServer;

// Internal Class instances 
EepromController eepromController;
MdnsController mdnsController;
OTAController otaController;
WifiController wifiController;

/*
 * Constructor
 *
 */
Framework::Framework(AsyncWebServer& server)  : _server(server) {}

/*
 * Shows "Wifi SSID & pass form page"
 */
void Framework::wifiSelectOptions(AsyncWebServerRequest *request) {
    request->send(200, "text/html", wifiOptions);
}

void Framework::wifiSelect(AsyncWebServerRequest *request) {
    String filePath = "/nonConnected/layout.html.gz";
    
    if (!LittleFS.exists(filePath)) { 
      request->send(404, "text/plain", "Page not found");
    }

    AsyncWebServerResponse *response = request->beginResponse(LittleFS, filePath, "text/html");
    response->addHeader("Content-Encoding", "gzip");
    
    request->send(response); 
}



/*
 * Sets WiFi Credentials in EEPROM
 */
void Framework::setWifiCreds(AsyncWebServerRequest *request) {
    // Get input credentials
    String ssidInput = request->arg("ssid");
    String passInput = request->arg("pass");

    eepromController.wipe();
    eepromController.storeWifiCreds(ssidInput, passInput);

    String filePath = "/nonConnected/RestartPage.html";

    if (!LittleFS.exists(filePath)) {
      request->send(404, "text/plain", "Page not found");
    }

    request->send(LittleFS, filePath, "text/html");

    delay(10000);

    ESP.restart();
}

/*
 * Routes for not connected to WiFi state
 */
void Framework::notConnectedRoutes() {
  _server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->wifiSelect(request);
  });

  _server.on("/generate_204", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->wifiSelect(request);
  });

_server.on("/captive.apple.com", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->wifiSelect(request);
  });

  _server.on("/fwlink", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->wifiSelect(request);
  });

  _server.on("/wificreds", HTTP_POST, [this](AsyncWebServerRequest *request){
    this->setWifiCreds(request);
  });

  _server.on("/ssid_options", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->wifiSelectOptions(request);
  });
}

/*
 * Setup WiFi main function
 */
void Framework::setupCredsRoutine() {
  wifiOptions = wifiController.getSsidOptions();
  wifiController.setupWifiAp(DEVICE_NAME);
  dnsServer.start(DNS_PORT, "*", dns_IP);
  
  notConnectedRoutes();

  // Reply to all requests with same HTML
  _server.onNotFound([this](AsyncWebServerRequest *request){
      this->wifiSelect(request);
  });
}

/*********************************************************************/


/*
 * Root for connected state
 */
void Framework::landing(AsyncWebServerRequest *request) {
    // If no pass has been set, return the set password page, otherwise return the login page 
    if(eepromController.isAdminPassSet() == "1") {
         if (!LittleFS.exists("/connected/AdminLoginPage.html")) {
          request->send(404, "text/plain", "Page not found");
        }
        request->send(LittleFS, "/connected/AdminLoginPage.html", "text/html");
    } else {
        if (!LittleFS.exists("/connected/AdminSetPassPage.html")) {
          request->send(404, "text/plain", "Page not found");
        }
        request->send(LittleFS, "/connected/AdminSetPassPage.html", "text/html");

    }
}

/*
 * Clear EEPROM page
 */
void Framework::clearEepromFull(AsyncWebServerRequest *request) {
   
    if (!LittleFS.exists("/ClearEepromPage.html")) {
      request->send(404, "text/plain", "Page not found");
    }

    request->send(LittleFS, "/ClearEepromPage.html", "text/html");
    eepromController.wipe();
    wifiController.forgetWifi();
    delay(5000);
    
    ESP.restart();
}


/*
 * Clear the admin pass word and isSet flag from eeprom
 */
void Framework::clearEepromAdminPass(AsyncWebServerRequest *request) {
  
    eepromController.clearAdminPass();
    request->redirect("/");
    request->send( 302, "text/plain", "");
}

/*
 * Sets Configuration password in EEPROM
 */
void Framework::setAdminPass(AsyncWebServerRequest *request) {
  if(eepromController.isAdminPassSet() != "1"){
    String pass = request->arg("pass");
    String confirm = request->arg("confirm");

    // Validate (TODO :: need to validate not empty fields & standard password stuff)
    if(pass.equals(confirm)){
      eepromController.storeAdminPass(pass);
    }
  }
  
  request->redirect("/");
  request->send(302, "text/plain", "");
  
}


/*
 * Admin Login -- validates the admin login password, if incorrect sends the user to the root URL or if correct sends the user to the dashboard 
 */
 void Framework::adminLogin(AsyncWebServerRequest *request) {

    String returnRoute;

    if(eepromController.isAdminPassSet() == "1"){
      String userPass = request->arg("pass");
      String eepromPass = eepromController.getAdminPass();

      if(eepromPass.equals(userPass)){
         returnRoute = loggedInRoute;
      }else{
         returnRoute = "/";
      }
      
    }else{
      returnRoute = "/";  
    }
    
    request->redirect(returnRoute);
    request->send( 302, "text/plain", "");
 }


/*
 * Sets the routes for connected state
 */
void Framework::connectedRoutes() {
  _server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->landing(request);
  });

  _server.on("/eeprom/clear/all", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->clearEepromFull(request);
  });

  _server.on("/eeprom/clear/adminpass", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->clearEepromAdminPass(request);
  });

  _server.on("/admin/set/pass", HTTP_POST, [this](AsyncWebServerRequest *request){
    this->setAdminPass(request);
  });

  _server.on("/admin/login", HTTP_POST, [this](AsyncWebServerRequest *request){
    this->adminLogin(request);
  });
}

void Framework::serveHtmx(){
   _server.on("/htmx", HTTP_GET, [](AsyncWebServerRequest *request){
    String filePath = "/htmx.min.js.gz";

    if (!LittleFS.exists(filePath)) { 
      request->send(404, "text/plain", "Page not found");
      return;
    }

    AsyncWebServerResponse *response = request->beginResponse(LittleFS, filePath, "application/gzip");
    response->addHeader("Content-Encoding", "gzip");
    
    request->send(response);

  }); 
}

void Framework::startEeprom() {
    EEPROM.begin(512);
}

/*
 * Main Setup
 */
void Framework::begin() {
   
    #ifdef SERIAL_DEBUGGING
      Serial.begin(115200);
      Serial.println();
    #endif

    if (!LittleFS.begin()) {
      #ifdef SERIAL_DEBUGGING
        Serial.println("Failed to mount file system");
      #endif

      return;
    }

   eepromController.startEeprom();   
   delay(1000);

  serveHtmx();

   if(!wifiController.wifiConnTimer(WIFI_CON_WAIT, eepromController.eepromGetWifiSsid(), eepromController.eepromGetWifiPass(), DEVICE_NAME) || WiFi.status() != WL_CONNECTED){
      setupCredsRoutine();
      #ifdef SERIAL_DEBUGGING
        Serial.println("please connect to WiFi");
      #endif
   }else{ 
      #ifdef SERIAL_DEBUGGING
        Serial.println("Connected  :)");
        Serial.println(WiFi.localIP()); 
      #endif
      connectedRoutes();
      mdnsController.initMdns(DEVICE_NAME);
      otaController.initOTA(DEVICE_NAME);
   }
  _server.begin(); 
}

/*
 * Main Loop
 */
void Framework::run() {
    
    if(WiFi.status() != WL_CONNECTED){
      dnsServer.processNextRequest();
    }else{
      otaController.handleOTA();
      mdnsController.loopHandle();
    }
}
