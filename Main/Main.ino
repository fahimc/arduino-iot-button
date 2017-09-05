#include "src/WIFISetup.h";
#include "src/SetupServer.h";

WIFISetup wifiSetup = WIFISetup();
SetupServer* setupServer;

void setup() {
  wifiSetup.init();
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  wifiSetup.scan();
  setupServer = new SetupServer();
  setupServer->updateSSIDList(wifiSetup.getSSIDList());
  setupServer->start();
}
