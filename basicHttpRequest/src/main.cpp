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
#include <cJSON.h>

#define TFT_CS 12   // Pins for the SPI interface
#define TFT_RST 3
#define TFT_DC 2
#define TFT_MOSI 11
#define TFT_SCLK 7

class Adafruit_ST7735Ext : public Adafruit_ST7735{    // Extend the display library to be able to cleanly add functionality
  public:
    Adafruit_ST7735Ext (int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : Adafruit_ST7735(cs, dc, mosi, sclk, rst){}  // Same constructor as the class it's extending

    void movingText(String toWrite, int y){
      int pixelLength = toWrite.length() * 6;                       // 6 pixels width each character
      int extraPixels = (ceil((pixelLength - 108)/6) * 6)+1;
      fillRect(1, y, 126, 8, ST7735_BLACK);                  // black out the line where the text will be
      setCursor(10, y);
      print(toWrite);
      fillRect(1, y, 10, 8, ST7735_BLACK);                   // black out the edges of the text
      fillRect(117, y, 10, 8, ST7735_BLACK);
      delay(3000);                                                  // time to read the first part of the text
      for(int i = 9; i > -extraPixels; i--){
        fillRect(1, y, 128, 8, ST7735_BLACK);
        setCursor(i, y);                                     // move 1 pixel to the left
        print(toWrite);
        fillRect(1, y, 10, 8, ST7735_BLACK);
        fillRect(117, y, 10, 8, ST7735_BLACK);
        drawRect(0, 0, 128, 160, ST77XX_YELLOW);
        delay(100);
      }
    }
};

class ProfileDetails{
  public:
    ProfileDetails(const char *value){
      profile = cJSON_Parse(value);
    }

    int getId(){
      return profile->child->valueint;
    }

    String getCurrency(){
      return profile->child->next->valuestring;
    }

    float getTotalValue(){
      return profile->child->next->next->valuedouble;
    }

    float getCashAvailableToTrade(){
      return profile->child->next->next->next->child->valuedouble;
    }

    float getCashReservedForOrders(){
      return profile->child->next->next->next->child->next->valuedouble;
    }

    float getCashInPies(){
      return profile->child->next->next->next->child->next->next->valuedouble;
    }

    float getInvestmentsCurrentValue(){
      return profile->child->next->next->next->next->child->valuedouble;
    }

    float getInvestmentsTotalCost(){
      return profile->child->next->next->next->next->child->next->valuedouble;
    }

    float getInvestmentsRealisedProfitLoss(){
      return profile->child->next->next->next->next->child->next->next->valuedouble;
    }

    float getInvestMentsUnrealisedProfitLoss(){
      return profile->child->next->next->next->next->child->next->next->next->valuedouble;
    }
  
  private:
    cJSON* profile;
};

Adafruit_ST7735Ext tft = Adafruit_ST7735Ext(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const char *ssid = SSID;      // This stuff is set in secret.h
const char *password = PASS;

// Function declarations
void jsonOutput(String);

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
      ProfileDetails parsed = ProfileDetails(payload.c_str());
      // Serial.println(parsed.getId());
      tft.movingText("Id: "+(String)parsed.getId(), 30);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
  delay(10000);
}