#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <math.h>
#include <base64.hpp>
#include <string.h>
#include <cJSON.h>
#include <Adafruit_ST7735Ext.h>
#include <jsonGetters.h>

Adafruit_ST7735Ext tft = Adafruit_ST7735Ext(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  Serial.begin(115200);
  delay(1000);  //Delay so that we have time to open the serial monitor
  tft.initR(INITR_BLACKTAB);      // Initialise the displayo object
  tft.setRotation(2);             // The screen is portrait
  tft.fillScreen(ST77XX_BLACK);   // Black out the screen
  tft.setTextWrap(false);         // This is so that we can use the moving text

  Serial.println();
  Serial.println("******************************************************");
  Serial.println("Connecting to ");
  Serial.println(SSID);
  WiFi.setHostname("MyEsp32");  //This is visible on the network that you connect to

  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.logo();
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  tft.fillScreen(ST7735_BLACK);
  tft.drawRect(0, 0, 128, 160, TRADING21BLUE);
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;  //Start a HTTP client to be able to send http requests
    // http.begin("https://live.trading212.com/api/v0/equity/account/summary");  // Access account data
    http.begin("https://live.trading212.com/api/v0/equity/metadata/instruments?limit=1"); //Get all available instruments
    http.addHeader("Authorization", "Basic " + String((char *) encoded));     // Authorisation header
    
    int httpCode = http.GET();  //Send get request
    if(httpCode > 0){
      String payload = http.getString();  //Get the response
      Serial.println(httpCode);           //Print the response code
      http.end();
      // payload = payload + "]";
      // ProfileDetails parsed = ProfileDetails(payload.c_str());
      Serial.println(payload);
      // Serial.println(cJSON_Print(cJSON_Parse(payload.c_str())));

      // Serial.println(cJSON_Print(cJSON_GetArrayItem(cJSON_Parse(payload.c_str()), 0)));
      tft.movingText("Check serial", 50);
    } else {
      Serial.println("Error on HTTP request");
      http.end();
    }
  }
  delay(20000);
}