#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#include <math.h>
#include <base64.hpp>
#include <string.h>
#include <cJSON.h>
#include <Adafruit_ST7735Ext.h>
#include <jsonGetters.h>

Adafruit_ST7735Ext tft = Adafruit_ST7735Ext(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

String inputWord;
String retVal;
String mySsid;
bool mySsidDone = false;
String myPass;
bool myPassDone = false;

String takeInput(Adafruit_ST7735Ext* myTft){
  String in;
  String tftReturn;
  String toReturn;
  bool done = false;
  while(!done){
    while(Serial.available()){
      in = Serial.readString();
      if (in == "l"){
        (*myTft).changeLetter(LEFT);
      } else if (in == "u"){
        (*myTft).changeLetter(UP);
      } else if (in == "r"){
        (*myTft).changeLetter(RIGHT);
      } else if (in == "d"){
        (*myTft).changeLetter(DOWN);
      } else if (in == "s"){
        tftReturn = (*myTft).inputKey(toReturn);
        if(!tftReturn.isEmpty() or tft.getCurrentLetter()==BACKSPACE){
          toReturn = tftReturn;
        }
        Serial.println(toReturn);
        if((*myTft).getCurrentLetter() == ENTER) done = true;
      }
    }
  }
  return toReturn;
}

void setup() {
  Serial.begin(115200);
  delay(1000);  //Delay so that we have time to open the serial monitor
  tft.initR(INITR_BLACKTAB);      // Initialise the display object
  tft.setRotation(2);             // The screen is portrait
  tft.fillScreen(ST77XX_BLACK);   // Black out the screen
  tft.setTextWrap(false);         // This is so that we can use the moving text
  tft.fillScreen(ST7735_BLACK);
  tft.logo();
  delay(1000);
  tft.fillScreen(ST7735_BLACK);
  tft.putKeyboard(111, false);

  mySsid = takeInput(&tft);
  
  Serial.println("Ready for password");
  
  myPass = takeInput(&tft);

  Serial.println();
  Serial.println("******************************************************");
  Serial.println("Connecting to ");
  Serial.println(mySsid);
  WiFi.setHostname("MyEsp32");  //This is visible on the network that you connect to

  WiFi.begin(mySsid, myPass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.logo();
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // tft.fillScreen(ST7735_BLACK);
  // tft.drawRect(0, 0, 128, 160, TRADING21BLUE);
  // tft.putKeyboard(111, false);
}

void loop() {
  // if(WiFi.status() == WL_CONNECTED){
  //   HTTPClient http;  //Start a HTTP client to be able to send http requests
  //   // http.begin("https://live.trading212.com/api/v0/equity/account/summary");  // Access account data
  //   http.begin("https://live.trading212.com/api/v0/equity/metadata/instruments?limit=1"); //Get all available instruments
  //   http.addHeader("Authorization", "Basic " + String((char *) encoded));     // Authorisation header
    
  //   int httpCode = http.GET();  //Send get request
  //   if(httpCode > 0){
  //     String payload = http.getString();  //Get the response
  //     Serial.println(httpCode);           //Print the response code
  //     http.end();
  //   } else {
  //     Serial.println("Error on HTTP request");
  //     http.end();
  //   }
  // }

  // delay(5000);
}