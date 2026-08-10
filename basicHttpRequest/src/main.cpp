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

    tft.setCursor(5, 45);
    tft.setFont(&Keyboard);
    tft.println("Password:");
    tft.setFont();
    myPass = tft.takeInput(5, 60);
    preferences.putString("pass", myPass);
  }

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

    tft.setCursor(5, 45);
    tft.setFont(&Keyboard);
    tft.println("API Key:");
    tft.setFont();
    myKey = tft.takeInput(5, 60);
    preferences.putString("key", myKey);
  }


  apiPair = preferences.getString("id") +":"+ preferences.getString("key");
  int len = encode_base64((unsigned char*) apiPair.c_str(), apiPair.length(), encoded);
  encodedPair = String((char*)encoded).substring(0, len);

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
}

void loop()
{
  if(WiFi.status() == WL_CONNECTED){
    tft.setCursor(5, 10);
    tft.printUnderlineDefaultFont("Summary", ST7735_WHITE);
    tft.printCentreLeftAlign("The quick brown fox jumped over the lazy dog", 20);
  }
  delay(5000);
}