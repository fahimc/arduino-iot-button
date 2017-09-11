#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <EEPROM.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
int buttonState = 0; 
int readyState = 0;
WiFiManagerParameter wifiParameter("actionURL","set action url","",200);

void saveURL(String url){
  int charLength=url.length();
  
  Serial.println("writing eeprom ssid:");
          for (int i = 0; i < 200; ++i)
            {
              EEPROM.write(i, url[i]);
              Serial.print("Wrote: ");
              Serial.println(url[i]); 
            }

}

String readUrl(){
  String url;
  for (int i = 0; i < 200; ++i)
    {
      char _byte = char(EEPROM.read(i));
      url += _byte;  
    }
    //esid.trim();
    Serial.println(url.length());
  Serial.print("URL: ");
  Serial.println(url);

  return url;
}

void makeRequest(){
  //String url = readUrl();
  String url = "http://maker.ifttt.com/trigger/button_pressed/with/key/dnZFBpYIBLexQjFcj2zXb33bEQnB5xQaiRrrSEa1-_s";
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
  digitalWrite(D2, HIGH);
}

void saveConfigCallback(){
  Serial.println("get param value");
  Serial.println(wifiParameter.getValue());
  //saveURL(wifiParameter.getValue());
}


void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //EEPROM.begin(512);
  pinMode(D2, OUTPUT);
  pinMode(D1, INPUT);
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
  digitalWrite(D2, HIGH);
}



void loop() {
  // put your main code here, to run repeatedly:
  int previousButtonState = buttonState;
  buttonState = digitalRead(D1);
  if(readyState && previousButtonState != buttonState)
  {
    digitalWrite(D2, LOW);
    makeRequest();
  }

}
