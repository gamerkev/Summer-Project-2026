#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include "secret.h"
#include <math.h>
#include <base64.hpp>
#include <string.h>
#include <cJSON.h>
#include <Adafruit_ST7735Keyboard.h>
#include <jsonGetters.h>
#include <Preferences.h>
#include <string.h>
#include <poundSign.h>

Preferences preferences;

Adafruit_ST7735Keyboard tft = Adafruit_ST7735Keyboard(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

String mySsid;
String myPass;
String myId;
String myKey;
String apiPair;
unsigned char encoded[200];
String encodedPair;

void setup()
{
  Serial.begin(115200);
  delay(1000);                  // Delay so that we have time to open the serial monitor
  tft.initR(INITR_BLACKTAB);    // Initialise the display object
  tft.setRotation(2);           // The screen is portrait
  tft.fillScreen(ST77XX_BLACK); // Black out the screen
  tft.setTextWrap(false);       // This is so that we can use the moving text
  tft.fillScreen(ST7735_BLACK);
  tft.logo();
  delay(1000);

  preferences.begin("netCreds", false);
  Serial.println("Begun netcreds");
  if (preferences.getString("ssid").isEmpty() or preferences.getString("pass").isEmpty())
  {
    tft.fillScreen(ST7735_BLACK);
    tft.putKeyboard(111, false);
    tft.setCursor(5, 15);
    tft.setFont(&Keyboard);
    tft.println("ssid:");
    tft.setFont();

    mySsid = tft.takeInput(5, 30);
    preferences.putString("ssid", mySsid);

    Serial.println("Ready for password");

    tft.setCursor(5, 45);
    tft.setFont(&Keyboard);
    tft.println("Password:");
    tft.setFont();
    myPass = tft.takeInput(5, 60);
    preferences.putString("pass", myPass);
  }
  Serial.println("Checked network stuff");

  if (preferences.getString("id").isEmpty() or preferences.getString("key").isEmpty())
  {
    tft.fillScreen(ST7735_BLACK);
    tft.putKeyboard(111, false);
    tft.setCursor(5, 15);
    tft.setFont(&Keyboard);
    tft.println("API ID:");
    tft.setFont();

    myId = tft.takeInput(5, 30);
    preferences.putString("id", myId);

    Serial.println("Ready for API key");

    tft.setCursor(5, 45);
    tft.setFont(&Keyboard);
    tft.println("API Key:");
    tft.setFont();
    myKey = tft.takeInput(5, 60);
    preferences.putString("key", myKey);
  }
  Serial.println("Checked API stuff");


  apiPair = preferences.getString("id") +":"+ preferences.getString("key");
  Serial.println("Created API pair");
  int len = encode_base64((unsigned char*) apiPair.c_str(), apiPair.length(), encoded);
  encodedPair = String((char*)encoded).substring(0, len);

  Serial.println("Encoded creds");

  Serial.println();
  Serial.println("******************************************************");
  Serial.println("Connecting to ");
  Serial.println(preferences.getString("ssid"));
  WiFi.begin(preferences.getString("ssid"), preferences.getString("pass"));

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    tft.logo();
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  tft.setCursor(5, 80);
  tft.println("connected");
  delay(2000);
  tft.fillScreen(ST7735_BLACK);
  tft.drawRect(0, 0, 128, 160, TRADING21BLUE);
  // tft.putKeyboard(111, false);
}

void loop()
{
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;  //Start a HTTP client to be able to send http requests
    http.begin("https://live.trading212.com/api/v0/equity/account/summary");  // Access account data
    // http.begin("https://live.trading212.com/api/v0/equity/metadata/instruments?limit=1"); //Get all available instruments
    http.addHeader("Authorization", "Basic " + encodedPair);     // Authorisation header

    int httpCode = http.GET();  //Send get request
    if(httpCode > 0){
      String payload = http.getString();  //Get the response
      // Serial.println(httpCode);           //Print the response code
      // Serial.println(payload);            //Print the payload
      http.end();
      extractData payloadJson = extractData(payload.c_str());
      String curr = payloadJson.getCurrency();
      char currSym = currencySymbol(curr);
      int startLevel = 14;
      tft.setCursor(10,10);
      tft.setFontSameSize(&Pound);
      tft.print("£");
      tft.setFontSameSize(NULL);
      tft.print("10");
      // tft.movingText("Total account value:", startLevel);
      // tft.movingText(currSym+String(payloadJson.getTotalValue()), startLevel+13);
      // tft.movingText("Cash available to trade:", startLevel+13+15);
      // tft.movingText(currSym+String(payloadJson.getCashAvailableToTrade()), startLevel+26+15);
      // tft.movingText("Current value of investments:", startLevel+26+30);
      // tft.movingText(currSym+String(payloadJson.getInvestmentsCurrentValue()), startLevel+39+30);
      // tft.movingText("Total cost of investments:", startLevel+39+45);
      // tft.movingText(currSym+String(payloadJson.getInvestmentsTotalCost()), startLevel+52+45);
      // tft.movingText("Unrealised profit/loss:", startLevel+52+60);
      // tft.movingText(currSym+String(payloadJson.getInvestMentsUnrealisedProfitLoss()), startLevel+65+60);
    } else {
      Serial.println("Error on HTTP request: " + httpCode);
      http.end();
    }
  }

  delay(5000);
}