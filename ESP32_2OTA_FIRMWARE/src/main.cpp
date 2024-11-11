#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>

#include <http_ota.h>


// WiFi&OTA 参数
String HOSTNAME = "";
#define PASSWORD "green7650" // the password for OTA upgrade, can set it in any char you want

const char* ssid = "greenio";
const char* password = "green7650";

long Time = 0;

void setupOTA()
{
  ArduinoOTA.setHostname(HOSTNAME.c_str());
  ArduinoOTA.setPassword(PASSWORD);
  ArduinoOTA.onStart([]()
                     {    
                      //  lcd.begin(COLUMS, ROWS);
                       Serial.println("Start Updating....");
                       Serial.printf("Start Updating....Type:%s\n", (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem"); });

  ArduinoOTA.onEnd([]()
                   {
                     Serial.println("Update Complete!");
                     // lcd.clear();
                     // lcd.setCursor(0, 0);
                     // lcd.print("Update Complete!");
                   });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        {
                          String pro = String(progress / (total / 100)) + "%";
                          int progressbar = (progress / (total / 100));
                          Serial.print("Progress : ");
                          Serial.println((progress / (total / 100)));

                          // lcd.clear();
                          // lcd.setCursor(0, 0);
                          // lcd.print("Update :");
                          // lcd.setCursor(9, 0);
                          // lcd.print(progress / (total / 100));
                        });

  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    String info = "Error Info:";
    switch (error)
    {
      case OTA_AUTH_ERROR:
        info += "Auth Failed";
        Serial.println("Auth Failed");
        break;

      case OTA_BEGIN_ERROR:
        info += "Begin Failed";
        Serial.println("Begin Failed");
        break;

      case OTA_CONNECT_ERROR:
        info += "Connect Failed";
        Serial.println("Connect Failed");
        break;

      case OTA_RECEIVE_ERROR:
        info += "Receive Failed";
        Serial.println("Receive Failed");
        break;

      case OTA_END_ERROR:
        info += "End Failed";
        Serial.println("End Failed");
        break;
    }
    Serial.println(info);
    ESP.restart(); });
  ArduinoOTA.begin();
}

void connect_wifi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();
  }
  else
  {
    OTA_git_CALL();
  }
}

void setup() {
// OTA NAME_Ver  
  Project = "HTTPS_OTA_ESP32";
  FirmwareVer = "0.1";

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  connect_wifi();
  Serial.println("Connected to WiFi");
  setupOTA();

}

void loop() {
  if(millis()-Time >1000){
    Serial.println("FirmwareVer :" + String(FirmwareVer));
    OTA_git_CALL();
  Time = millis();
  }
  ArduinoOTA.handle();
}