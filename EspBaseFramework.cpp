#define SERIAL_DEBUGGING

// Import pages
#include "Pages/WifiFormPage1.h"
#include "Pages/WifiFormPage2.h"
#include "Pages/ClearEepromPage.h"
#include "Pages/RestartPage.h"
#include "Pages/AdminSetPassPage.h"
#include "Pages/AdminLoginPage.h"

#include "EspBaseFramework.h"

/* Pages declaration */
//  WiFi connected
const String adminSetPassPage = FPSTR(ADMINSETPASS_page);
const String adminLoginPage = FPSTR(ADMINLOGIN_page);
const String clearEepromPage = FPSTR(CLEAREEPROM_page);

//  Non WiFi connected 
const String wifiFormPage1 = FPSTR(WIFIFORM_page_1);
const String wifiFormPage2 = FPSTR(WIFIFORM_page_2);
const String RestartPage = FPSTR(RESTART_page);


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
 */
Framework::Framework(AsyncWebServer& server)  : _server(server) {}

/*
 * Shows "Wifi SSID & pass form page"
 */
void Framework::wifiSelect(AsyncWebServerRequest *request) {
    String page = wifiFormPage1 + wifiOptions + wifiFormPage2;
    request->send_P(200, "text/html", page.c_str());
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

    request->send_P(200, "text/html", RestartPage.c_str());

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
   _server.onNotFound([](AsyncWebServerRequest *request){
    String page = wifiFormPage1 + wifiOptions + wifiFormPage2;
    request->send_P(200, "text/html", page.c_str());
  });
}

/*********************************************************************/


/*
 * Root for connected state
 */
void Framework::landing(AsyncWebServerRequest *request) {
    // If no pass has been set, return the set password page, otherwise return the login page 
    if(eepromController.isAdminPassSet() == "1") {
        request->send_P(200, "text/html", adminLoginPage.c_str());
    } else {
        request->send_P(200, "text/html", adminSetPassPage.c_str());
    }
}

/*
 * Clear EEPROM page
 */
void Framework::clearEepromFull(AsyncWebServerRequest *request) {
     eepromController.wipe();
     request->send(200, "text/html", clearEepromPage);
     wifiController.forgetWifi();
     delay(10000);
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
 // Check if the admin pass is NOT set
  if(eepromController.isAdminPassSet() != "1"){
  
    // Get input credentials
    String pass = request->arg("pass");
    String confirm = request->arg("confirm");

    // Validate (TODO :: need to validate not empty fields & standard password  stuff )
    if(pass.equals(confirm)){
      eepromController.storeAdminPass(pass);
    }
  }
  
  // Send back to main root page to login 
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

   eepromController.startEeprom();   
   delay(1000);

   String ssid = eepromController.eepromGetWifiSsid();
   String pass = eepromController.eepromGetWifiPass();
   wifiController.wifiConnTimer(WIFI_CON_WAIT, ssid, pass, DEVICE_NAME);

   if(WiFi.status() != WL_CONNECTED){
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
    dnsServer.processNextRequest();

    if(WiFi.status() != WL_CONNECTED){
      // Do Nothing
    }else{
     otaController.handleOTA();
     mdnsController.loopHandle();
    }
}
