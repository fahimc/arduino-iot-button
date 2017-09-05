#include "ESP8266WiFi.h";

class WIFISetup {
  char** SSID_LIST;
public:
  WIFISetup (){ 
  }
  void init(){
    Serial.begin(115200);
    setWIFIToStationMode();
    Serial.println("Setup done");
  }
  void setWIFIToStationMode(){
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
  }
  char** getSSIDList(){
    return SSID_LIST;
  }
  void scan(){
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
    Serial.println("no networks found");
    else
    {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      String item = WiFi.SSID(i);
      Serial.println("Goat1");
      //Serial.println(SSID_LIST);
      item.toCharArray(SSID_LIST[i], item.length());
      Serial.println("Goat3");
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println(WiFi.encryptionType(i));
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
  }
};
