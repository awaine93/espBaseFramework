#include <EEPROM.h>
#include "EepromController.h" 

#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif


/*
 * Start EEPROM
 */
void EepromController::startEeprom(){
     EEPROM.begin(512);
}


/*
 * Clears EEPROM
 */
void EepromController::wipe(){
  
   for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, -1);
      }

       EEPROM.commit();
}

/*
 * Sotes Wifi Credentials
 */
void EepromController::storeWifiCreds(String ssid,String pass){

  
// Declare char arrays
  char ssidArray[50];
  char passArray[50];

  // Convert to char array
  ssid.toCharArray(ssidArray, ssid.length()+1);
  pass.toCharArray(passArray, pass.length()+1);


  //TODO:: DELETE THIS ON PRODUCTION
  Serial.println(ssidArray);
  Serial.println(passArray);


  EEPROM.put(0, ssidArray);
  EEPROM.put(50, passArray);
  EEPROM.commit();


}


/*
 * Gets WiFi ssid from EEPROM
 */
 String EepromController::eepromGetWifiSsid(){

 // Declare char arrays
  char ssidArray[50];

  EEPROM.get(0, ssidArray);
    
  return ssidArray;
 }

/*
 * Gets WiFi pass from EEPROM
 */
 String EepromController::eepromGetWifiPass(){

 // Declare char arrays
  char passArray[50];

  EEPROM.get(50, passArray);
  
  return passArray;
 }


/*
 * Sotes Configuration Cassword
 */
void EepromController::storeAdminPass(String pass){

  
// Declare char arrays
  char passArray[50];

  // Convert to char array
  pass.toCharArray(passArray, pass.length()+1);
 
  EEPROM.put(100, "1");
  EEPROM.put(101, passArray);
  EEPROM.commit();
}

 

/*
 * Gets Configuration password from EEPROM
 */
 String EepromController::isAdminPassSet(){

 // Declare char arrays
  char passCheck[1];

  EEPROM.get(100, passCheck);
  
  return passCheck;
 }


/*
 * Gets Configuration password from EEPROM
 */
 String EepromController::getAdminPass(){

 // Declare char arrays
  char passArray[50];

  EEPROM.get(101, passArray);

  return passArray;
 }


 /*
 * Clears Admin Pass and IsSet flag from EEPROM
 */
void EepromController::clearAdminPass(){
  
   for (int i = 100 ; i < 151 ; i++) {
        EEPROM.write(i, -1);
   }

       EEPROM.commit();
}
