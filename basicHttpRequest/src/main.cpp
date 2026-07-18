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
    http.begin("https://live.trading212.com/api/v0/equity/account/summary");  // Trading212 api
    http.addHeader("Authorization", "Basic " + String((char *) encoded));     // Authorisation header
    
    int httpCode = http.GET();  //Send get request
    if(httpCode > 0){
      String payload = http.getString();  //Get the response
      Serial.println(httpCode);           //Print the response code
      ProfileDetails parsed = ProfileDetails(payload.c_str());
      // Serial.println(parsed.getId());
      tft.movingText("Hello id"+(String)parsed.getId(), 10);
      tft.movingText("Currency: " + (String)parsed.getCurrency(), 18);
      tft.movingText("Total value: " + (String)parsed.getTotalValue(), 26);
      tft.movingText("Cash available to trade: " + (String)parsed.getCashAvailableToTrade(), 34);
      tft.movingText("Cash reserved for orders: " + (String)parsed.getCashReservedForOrders(), 42);
      tft.movingText("Cash in pies: " + (String)parsed.getCashInPies(), 50);
      tft.movingText("Current value of investments: " + (String)parsed.getInvestmentsCurrentValue(), 58);
      tft.movingText("Total cost of investments: " + (String)parsed.getInvestmentsTotalCost(), 66);
      tft.movingText("Realised profit loss: " + (String)parsed.getInvestmentsRealisedProfitLoss(), 74);
      tft.movingText("Unrealised profit loss: " + (String)parsed.getInvestMentsUnrealisedProfitLoss(), 82);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
  delay(20000);
}