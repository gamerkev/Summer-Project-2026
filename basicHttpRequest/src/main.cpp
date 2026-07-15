// /*
//     Go to thingspeak.com and create an account if you don't have one already.
//     After logging in, click on the "New Channel" button to create a new channel for your data. This is where your data will be stored and displayed.
//     Fill in the Name, Description, and other fields for your channel as desired, then click the "Save Channel" button.
//     Take note of the "Write API Key" located in the "API keys" tab, this is the key you will use to send data to your channel.
//     Replace the channelID from tab "Channel Settings" and privateKey with "Read API Keys" from "API Keys" tab.
//     Replace the host variable with the thingspeak server hostname "api.thingspeak.com"
//     Upload the sketch to your ESP32 board and make sure that the board is connected to the internet. The ESP32 should now send data to your Thingspeak channel at the intervals specified by the loop function.
//     Go to the channel view page on thingspeak and check the "Field1" for the new incoming data.
//     You can use the data visualization and analysis tools provided by Thingspeak to display and process your data in various ways.
//     Please note, that Thingspeak accepts only integer values.

//     You can later check the values at https://thingspeak.com/channels/2005329
//     Please note that this public channel can be accessed by anyone and it is possible that more people will write their values.
//  */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <math.h>

#define TFT_CS 12
#define TFT_RST 3
#define TFT_DC 2
#define TFT_MOSI 11
#define TFT_SCLK 7

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const char *ssid = SSID;      //This stuff is set in secret.h
const char *password = PASS;


// Function declarations
void jsonOutput(String);
void movingText(String, int, Adafruit_ST7735*);
void showBirdDetails(String, Adafruit_ST7735*);

void setup() {
  Serial.begin(115200);
  delay(1000);  //Delay so that we have time to open the serial monitor
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(false);
  // tft.setSPISpeed(40000000); // This doesn't make it any faster
  tft.drawRect(0, 0, 128, 160, ST77XX_YELLOW);

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
  tft.setCursor(10, 10);
  tft.println("Connected!!");
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;  //Start a HTTP client to be able to send http requests
    // http.begin("https://meowfacts.herokuapp.com/"); //The request that will be sent
    http.begin("https://api.ebird.org/v2/data/obs/KZ/recent");
    // THIS WILL BE DIFFERENT THINGS TO TRY ONCE I GET TRADING212 API STUFF
    // http.addHeader(@"Basic", <username>:<password>);
    // http.addHeader("Authorization", <API key>);
    // http.addHeader("Authorization", )
    // http.setAuthorization("<API key>");
    // http.setAuthorization("<username>:<password>");

    ///////////////////////////MOST LIKELY THIS ONE///////////////////////////////
    // http.begin("https://live.trading212.com/api/v0/equity/history/orders");  //
    // http.setAuthorizationType("Basic");                                      //
    // http.setAuthorization("<username>", "<password>");                       //
    // http.addHeader("Authorization", "<API key>");                            //
    //////////////////////////////////////////////////////////////////////////////

    http.addHeader("X-eBirdApiToken", BIRD_API_TOKEN);
    int httpCode = http.GET();  //Send it as a get request
    if(httpCode > 0){
      String payload = http.getString();  //Get the response
      Serial.println(httpCode);           //Print the response code
      // Serial.println(payload);         //Print the response body
      payload = payload.substring(payload.indexOf("{")+1);
      payload = payload.substring(0, payload.indexOf("}"));
      showBirdDetails(payload, &tft);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
  delay(10000);
}

void jsonOutput(String response){
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

void showBirdDetails(String response, Adafruit_ST7735* tft){
  int commaLocation = response.indexOf(",");
  for (int i = 1; i < 13; i++){
    movingText(response.substring(0, commaLocation), 20 + (i*9), tft);
    response = response.substring(commaLocation+1);
    commaLocation = response.indexOf(",");
  }
}