/*
 * Author Alex Waine
 * 
 * Esp8266 Setup Firmware  
 * 
 * Version 1.1.0
 * 
*/
 
#include "WifiController.h" 
#include <WiFi.h>


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
 void WifiController::wifiConnTimer(int secs, String ssid, String pass, String deviceName){
  
  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceName);
  WiFi.begin(ssid, pass);

  // 20 Sec count down for WiFi connection
  int counter = 0;
  while(WiFi.status() != WL_CONNECTED){

    delay(1000);
    Serial.println(counter);
      if(counter == secs){
        break;
      }
     counter++;
  }

  Serial.printf("New hostname: %s\n", WiFi.getHostname());

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
      Serial.println(WiFi.SSID(i));
      html = html + "<option value='"+WiFi.SSID(i)+"'>" + WiFi.SSID(i)+"</option>";
    }
      return html;
}
