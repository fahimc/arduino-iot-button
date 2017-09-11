#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <EEPROM.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
int readyState = 0;
WiFiManagerParameter wifiParameter("actionURL","Set Action URL","",200);

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void saveURL(String url){
  int charLength=url.length();
  
  Serial.println("writing eeprom ssid:");
          for (int i = 0; i < 200; ++i)
            {
              EEPROM.write(i,  url[i]);
              Serial.print("Wrote: ");
              Serial.println(url[i]); 
            }

}

String readUrl(){
  String url;
  for (int i = 0; i < 200; ++i)
    {
      char _byte = char(EEPROM.read(i));
      if(_byte) {
        url += _byte; 
        Serial.println(_byte);  
      }
      
    }
    //esid.trim();
    Serial.println(url.length());
  Serial.print("URL: ");
  Serial.println(url);

  return url;
}

void saveConfigCallback(){
  Serial.println("get param value");
  Serial.println(wifiParameter.getValue());
  saveURL(wifiParameter.getValue());
}

void makeRequest(){
  String url = readUrl();
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  http.begin(url);
  Serial.print("[HTTP] GET...\n");
  //http.addHeader("Content-Type", "text/plain");
  int httpCode = http.GET();
  if(httpCode > 0) {
     // HTTP header has been send and Server response header has been handled
     Serial.printf("[HTTP] GET... code: %d\n", httpCode);

     // file found at server
     if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
            }
     } else {
       Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
   }
  http.end();
  digitalWrite(4, HIGH);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
   Serial.println("setup");
  EEPROM.begin(512);
   Serial.println("EEPROM");
  pinMode(4, OUTPUT);
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();
  wifiManager.addParameter(&wifiParameter);
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  } 

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  readyState = 1;
  digitalWrite(4, HIGH);
}

void loop() {
  if(readyState)
  {
    readyState=0;
    digitalWrite(4, LOW);
    makeRequest();
    ESP.deepSleep(0);
  }
  
}

