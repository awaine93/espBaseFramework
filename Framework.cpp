/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.4.0
 * 
*/

#define SERIAL_DEBUGGING


// Import pages
#import "Pages/WifiFormPage1.h"
#import "Pages/WifiFormPage2.h"
#import "Pages/ClearEepromPage.h"
#import "Pages/RestartPage.h"
#import "Pages/AdminSetPassPage.h"
#import "Pages/AdminLoginPage.h"
#import "Pages/DashboardPage.h"

#include "Framework.h"

/* Pages declaration */
//  WiFi connected
const String adminSetPassPage = FPSTR(ADMINSETPASS_page);
const String adminLoginPage = FPSTR(ADMINLOGIN_page);
const String clearEepromPage = FPSTR(CLEAREEPROM_page);
const String dashboardPage = FPSTR(DASHBOARD_page);

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
//AsyncWebServer server(80);

// Internal Class instances 
EepromController EepromController;
MdnsController MdnsController;
OTAController OTAController;
WifiController WifiController;

/*
 * Constructor
 */
Framework::Framework(AsyncWebServer& server)  : _server(server) {}


//AsyncWebServer& Framework::getServer(){
//  return _server;
//}

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

    EepromController.wipe();
    EepromController.storeWifiCreds(ssidInput, passInput);

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
   wifiOptions = WifiController.getSsidOptions();
   WifiController.setupWifiAp(DEVICE_NAME);
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
    if(EepromController.isAdminPassSet() == "1") {
        request->send_P(200, "text/html", adminLoginPage.c_str());
    } else {
        request->send_P(200, "text/html", adminSetPassPage.c_str());
    }
}

/*
 * Clear EEPROM page
 */
void Framework::clearEepromFull(AsyncWebServerRequest *request) {
     EepromController.wipe();
     request->send(200, "text/html", clearEepromPage);
     WifiController.forgetWifi();
     delay(10000);
     ESP.restart();
}


/*
 * Clear the admin pass word and isSet flag from eeprom
 */
void Framework::clearEepromAdminPass(AsyncWebServerRequest *request) {
  
    EepromController.clearAdminPass();
    request->redirect("/");
    request->send( 302, "text/plain", "");
}

/*
 * Sets Configuration password in EEPROM
 */
void Framework::setAdminPass(AsyncWebServerRequest *request) {
 // Check if the admin pass is NOT set
  if(EepromController.isAdminPassSet() != "1"){
  
    // Get input credentials
    String pass = request->arg("pass");
    String confirm = request->arg("confirm");

    // Validate (TODO :: need to validate not empty fields & standard password  stuff )
    if(pass.equals(confirm)){
      EepromController.storeAdminPass(pass);
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

    if(EepromController.isAdminPassSet() == "1"){
      String userPass = request->arg("pass");
      String eepromPass = EepromController.getAdminPass();

      if(eepromPass.equals(userPass)){
         returnRoute = "/dashboard";
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
 * Returns the dashboard page
 */
 void Framework::dashboard(AsyncWebServerRequest *request){
     // server.send(200, "text/html", LittleFS.open("Pages/dashboard.html", "r"));
     request->send_P(200, "html", dashboardPage.c_str());
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

  _server.on("/dashboard", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->dashboard(request);
  });
}


void Framework::startEeprom() {
    EEPROM.begin(512);
}

/*
 * Main Setup
 */
void Framework::setup() {
   
    #ifdef SERIAL_DEBUGGING
      Serial.begin(115200);
      Serial.println();
    #endif

   EepromController.startEeprom();   
   delay(1000);

   String ssid = EepromController.eepromGetWifiSsid();
   String pass = EepromController.eepromGetWifiPass();
   WifiController.wifiConnTimer(WIFI_CON_WAIT, ssid, pass, DEVICE_NAME);

   // WiFi Connected check 
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
      MdnsController.initMdns(DEVICE_NAME);
      OTAController.initOTA(DEVICE_NAME);
   }

 // Initialize LittleFS
  if (!LittleFS.begin()) {
    #ifdef SERIAL_DEBUGGING
      Serial.println("LittleFS initialization failed!");
    #endif
    return;
  }
  _server.begin(); 
}

/*
 * Main Loop
 */
void Framework::loop() {
    // Handles any incoming request
    dnsServer.processNextRequest();

    if(WiFi.status() != WL_CONNECTED){
      // Do Nothing
    }else{
     OTAController.handleOTA();
     MdnsController.loopHandle();
    }
}
