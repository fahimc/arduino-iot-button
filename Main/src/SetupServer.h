char WiFiAPPSK[] = "IOT_BUTTON";

class SetupServer {
char** SSID_LIST;
WiFiServer server = WiFiServer(80);
public:
  SetupServer(){
    init();
  }
  void init(){
  	setupWiFi();
  	server.begin();
  }
  void updateSSIDList(char** _SSID_LIST){
  	for(int i = 0; i < sizeof(_SSID_LIST); i++){
       SSID_LIST[i] = _SSID_LIST[i];
    }
  }
  void setupWiFi()
  {
    WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    String AP_NameString = "ESP8266 Thing " + macID;

    char AP_NameChar[AP_NameString.length() + 1];
    memset(AP_NameChar, 0, AP_NameString.length() + 1);

    for (int i=0; i<AP_NameString.length(); i++)
      AP_NameChar[i] = AP_NameString.charAt(i);

    WiFi.softAP(AP_NameChar, WiFiAPPSK);
  }
  void start(){
  	// Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
      return;
    }
    client.flush();
    String s = "HTTP/1.1 200 OK\r\n";

    s += "Content-Type: text/html\r\n\r\n";
    s += "<!DOCTYPE HTML>hello world\r\n<html>\r\n";
    delay(1000000);
  }
};
