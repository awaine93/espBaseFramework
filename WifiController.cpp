#include "WifiController.h" 

#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif

#define SERIAL_DEBUGGING

IPAddress selfIP(192, 168, 1, 1);


void WifiController::setupWifiAp(String deviceName){
    
   WiFi.mode(WIFI_AP);
   WiFi.softAPConfig(selfIP, selfIP, IPAddress(255, 255, 255, 0));
   WiFi.softAP(deviceName);

   delay(500);
}


/*
 * Forgets the wifi credentials 
 */
 void WifiController::forgetWifi(){
  
   WiFi.disconnect(true);
   WiFi.mode(WIFI_AP);
   WiFi.begin("", "");
 }

/*
 * Attempt to connect to Wifi with N second timer
 */
 bool WifiController::wifiConnTimer(int secs, String ssid, String pass, String deviceName){
  
  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceName);
  if(WiFi.begin(ssid, pass) == 4){
    return false;
  }

  // 20 Sec count down for WiFi connection
  int counter = 0;
  while(WiFi.status() != WL_CONNECTED){

    delay(1000);
    #ifdef SERIAL_DEBUGGING
      Serial.println(counter);
    #endif
      if(counter == secs){
        break;
      }
     counter++;
  }

  #ifdef SERIAL_DEBUGGING
    #ifdef ESP32
      Serial.printf("New hostname: %s\n", WiFi.getHostname());
    #else
      Serial.printf("New hostname: %s\n", WiFi.hostname().c_str());
    #endif
   #endif

  return true;
 }


/*
 * Scans for ssids and return html options string
 */
String WifiController::getSsidOptions(){

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    String html = "";
    int n = WiFi.scanNetworks();

    for (int i = 0; i < n; i++)
    {
      #ifdef SERIAL_DEBUGGING
        Serial.println(WiFi.SSID(i));
      #endif
      html = html + "<option value='"+WiFi.SSID(i)+"'>" + WiFi.SSID(i)+"</option>";
    }
    
    return html;
}
