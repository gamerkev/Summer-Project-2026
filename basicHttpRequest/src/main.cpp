#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <math.h>
#include <base64.hpp>
#include <string.h>
#include "base64.hpp"

#define TFT_CS 12   // Pins for the SPI interface
#define TFT_RST 3
#define TFT_DC 2
#define TFT_MOSI 11
#define TFT_SCLK 7

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const char *ssid = SSID;      // This stuff is set in secret.h
const char *password = PASS;

// Function declarations
void jsonOutput(String);
void movingText(String, int, Adafruit_ST7735*);

void setup() {
  Serial.begin(115200);
  delay(1000);  //Delay so that we have time to open the serial monitor
  tft.initR(INITR_BLACKTAB);      // Initialise the displayo object
  tft.setRotation(2);             // The screen is portrait
  tft.fillScreen(ST77XX_BLACK);   // Black out the screen
  tft.setTextWrap(false);         // This is so that we can use the moving text
                                  // although I may make it so that it's set to false at the beginning
                                  // of that function and reset to true at the end
  tft.drawRect(0, 0, 128, 160, ST77XX_YELLOW);  // I like to do this to be able to tell if edge pixels would change

  Serial.println();
  Serial.println("******************************************************");
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.setHostname("MyEsp32");  //This is visible on the network that you connect to

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;  //Start a HTTP client to be able to send http requests
    http.begin("https://live.trading212.com/api/v0/equity/account/summary");  // Trading212 api
    http.addHeader("Authorization", "Basic " + String((char *) encoded));     // Authorisation header
    
    int httpCode = http.GET();  //Send get request
    if(httpCode > 0){
      String payload = http.getString();  //Get the response
      Serial.println(httpCode);           //Print the response code
      Serial.println(payload);         //Print the response body
      movingText((String)httpCode, 30, &tft);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
  delay(10000);
}

void jsonOutput(String response){                             // This will likely be replaced by an actual json parsing library
  int commaLocation = response.indexOf(",");
  while (commaLocation != -1){
    Serial.println(response.substring(0, commaLocation));
    response = response.substring(commaLocation+1);
    commaLocation = response.indexOf(",");
  }
}

void movingText(String toWrite, int y, Adafruit_ST7735* tft){
  int pixelLength = toWrite.length() * 6;                       // 6 pixels width each character
  int extraPixels = (ceil((pixelLength - 108)/6) * 6)+1;
  (*tft).fillRect(1, y, 126, 8, ST7735_BLACK);                  // black out the line where the text will be
  (*tft).setCursor(10, y);
  (*tft).print(toWrite);
  (*tft).fillRect(1, y, 10, 8, ST7735_BLACK);                   // black out the edges of the text
  (*tft).fillRect(117, y, 10, 8, ST7735_BLACK);
  delay(3000);                                                  // time to read the first part of the text
  for(int i = 9; i > -extraPixels; i--){
    (*tft).fillRect(1, y, 128, 8, ST7735_BLACK);
    (*tft).setCursor(i, y);                                     // move 1 pixel to the left
    (*tft).print(toWrite);
    (*tft).fillRect(1, y, 10, 8, ST7735_BLACK);
    (*tft).fillRect(117, y, 10, 8, ST7735_BLACK);
    (*tft).drawRect(0, 0, 128, 160, ST77XX_YELLOW);
    delay(100);
  }
}
