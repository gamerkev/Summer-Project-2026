#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include "secret.h"
#include <math.h>
#include <base64.hpp>
#include <string.h>
#include <cJSON.h>
#include <Adafruit_ST7735Keyboard.h>
#include <Preferences.h>
#include <string.h>

#define LEFT "a"
#define UP "w"
#define RIGHT "d"
#define DOWN "s"
#define SELECT " "

typedef enum
{
  MENU_PAGE = 0,
  SUMMARY_PAGE = 1,
  POSITIONS_PAGE = 2,
} Page;

Page currentPage;
Page previousPage;
MenuSelection menuSelection;
SummarySelection summarySelection;
PositionsSelection positionsSelection;

Preferences preferences;

Adafruit_ST7735Keyboard tft = Adafruit_ST7735Keyboard(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

String mySsid;
String myPass;
String myId;
String myKey;
String apiPair;
unsigned char encoded[200];
String encodedPair;

String in;
Summary summary;
Positions *positions;
Positions *currentPositions;
cJSON *positionsJson;

int moveBy;
int positionSelected = -1;

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

  apiPair = preferences.getString("id") + ":" + preferences.getString("key");
  int len = encode_base64((unsigned char *)apiPair.c_str(), apiPair.length(), encoded);
  encodedPair = String((char *)encoded).substring(0, len);

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
  currentPage = MENU_PAGE;
  previousPage = SUMMARY_PAGE;
}

void loop()
{
  if (previousPage != currentPage)
  {
    switch (currentPage)
    {
    case MENU_PAGE:
      previousPage = currentPage;
      menuSelection = SUMMARY;
      tft.printMenu(menuSelection);
      while (currentPage == MENU_PAGE)
      {
        in = Serial.readString();
        if (in == UP)
        {
          menuSelection = (menuSelection == 0) ? OPEN_POSITIONS : MenuSelection((menuSelection - 1) % 2);
          tft.printMenu(menuSelection);
        }
        else if (in == DOWN)
        {
          menuSelection = MenuSelection((menuSelection + 1) % 2);
          tft.printMenu(menuSelection);
        }
        else if (in == SELECT)
        {
          switch (menuSelection)
          {
          case SUMMARY:
            currentPage = SUMMARY_PAGE;
            break;
          case OPEN_POSITIONS:
            currentPage = POSITIONS_PAGE;
            break;
          }
        }
      }
      break;
    case SUMMARY_PAGE:
      previousPage = currentPage;
      summarySelection = PIES;
      summary = Summary(encodedPair, &WiFi);
      tft.printSummary(summary, summarySelection);
      while (currentPage == SUMMARY_PAGE)
      {
        in = Serial.readString();
        if (in == LEFT)
        {
          summarySelection = (summarySelection == 0) ? PIES : SummarySelection((summarySelection - 1) % 2);
          tft.printSummary(summary, summarySelection);
        }
        else if (in == RIGHT)
        {
          summarySelection = SummarySelection((summarySelection + 1) % 2);
          tft.printSummary(summary, summarySelection);
        }
        else if (in == SELECT)
        {
          switch (summarySelection)
          {
          case PIES:
            Serial.println("Pies WIP");
            break;
          case MENU_SUMMARY:
            currentPage = MENU_PAGE;
            break;
          }
        }
      }
      break;
    case POSITIONS_PAGE:
      // REMEMBER TO DEREFERENCE THE POSITIONS LINKED LIST WHEN EXITING THIS PAGE
      previousPage = currentPage;
      positionsSelection = MENU_POSITIONS;
      positionsJson = getPositions(encodedPair, &WiFi);
      positions = makePositions(positionsJson);
      currentPositions = positions;
      tft.printPositions(currentPositions, positions->count, positionsSelection, positionSelected);
      while (currentPage == POSITIONS_PAGE)
      {
        in = Serial.readString();
        if (in == LEFT)
        {
          Serial.println("Left");
          if (positions->count - currentPositions->count >= 7)
          {
            for (int i = 0; i < 7; i++)
              currentPositions = currentPositions->prevPos;
          }
          else
          {
            for (int i = 0; i < (positions->count / 7) * 7; i++)
            {
              currentPositions = currentPositions->nextPos;
            }
          }
          tft.printPositions(currentPositions, positions->count, positionsSelection, positionSelected);
        }
        else if (in == RIGHT)
        {
          Serial.println("Right");
          if (positions->count - currentPositions->count >= 7)
          {
            for (int i = 0; i < 7; i++)
              currentPositions = currentPositions->prevPos;
          }
          else
          {
            for (int i = 0; i < (positions->count / 7) * 7; i++)
            {
              currentPositions = currentPositions->nextPos;
            }
          }
          tft.printPositions(currentPositions, positions->count, positionsSelection, positionSelected);
        }
        else if (in == SELECT)
        {
          Serial.println("Select");
          switch (positionsSelection)
          {
          case MENU_POSITIONS:
            currentPage = MENU_PAGE;
            freePositions(positions);
            break;
          }
        }
      }
      break;
    }
  }
}