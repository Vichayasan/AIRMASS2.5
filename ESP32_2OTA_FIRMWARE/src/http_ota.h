#include <Arduino.h>

#include "cert.h"
#include <WebServer.h>
#include <HTTPUpdate.h>


String Project ;
String FirmwareVer;
String URL_fw_Version ;

//#define URL_fw_Bin "https://raw.githubusercontent.com/greenioiot/depaArgricultureNode/main/firmware.bin"
String URL_fw_Bin ;

int FirmwareVersionCheck(void){
  URL_fw_Version = "https://raw.githubusercontent.com/Vichayasan/"+String (Project) +"/main/bin_version.txt" ;
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += URL_fw_Version;
  fwurl += "?";
  fwurl += String(rand());
  Serial.println(fwurl);
  WiFiClientSecure *client = new WiFiClientSecure;

  if (client)
  {
    //client->setCACert(rootCACertificate);
    client->setInsecure();

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;

    if (https.begin(*client, fwurl))
    { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      delay(100);
      httpCode = https.GET();
      delay(100);
      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
      }
      else
      {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;
  }

  if (httpCode == HTTP_CODE_OK) // if version received
  {
    payload.trim();
    if (payload.equals(FirmwareVer))
    {
      Serial.printf("\nDevice already on latest firmware version:%s\n", FirmwareVer);
      return 0;
    }
    else
    {
      Serial.println(payload);
      Serial.println("New firmware detected");
      return 1;
    }
  }
  return 0;
}

void firmwareUpdate(void){
    URL_fw_Bin = "https://raw.githubusercontent.com/Vichayasan/"+String (Project) +"/main/firmware.bin";
  WiFiClientSecure client;
  // client.setCACert(rootCACertificate);
  client.setInsecure();
  t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    break;
  }
}

void OTA_git_CALL(){
  if (FirmwareVersionCheck())
  {
    firmwareUpdate();
  }
}

