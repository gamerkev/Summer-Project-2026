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

const char *ssid = SSID;      //This stuff is set in secret.h
const char *password = PASS;

void setup() {
  Serial.begin(115200);
  delay(5000);  //Delay so that we have time to open the serial monitor

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
    http.begin("https://meowfacts.herokuapp.com/"); //The request that will be sent
    int httpCode = http.GET();  //Send it as a get request
    if(httpCode > 0){
      String payload = http.getString();  //Get the response
      Serial.println(httpCode);           //Print the response code as well as the body
      Serial.println(payload);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
  delay(10000);
}
