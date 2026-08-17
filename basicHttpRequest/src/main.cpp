#include <Arduino.h>
#include <base64.hpp>
#include <Preferences.h>
#include <PageHandler.h>

Page currentPage;
Page previousPage;

Preferences preferences;

Adafruit_ST7735Keyboard tft = Adafruit_ST7735Keyboard(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

String apiPair;
unsigned char encoded[200];
String encodedPair;

Summary summary;
Positions *positions;
cJSON *positionsJson;
bool positionsExists = false;

void setup()
{
  Serial.begin(115200);
  delay(1000);                  // Delay so that we have time to open the serial monitor
  tft.initR(INITR_BLACKTAB);    // Initialise the display object
  tft.setRotation(2);           // The screen is portrait
  tft.fillScreen(ST77XX_BLACK); // Black out the screen
  tft.setTextWrap(false);       // This is so that we can use the moving text
  tft.fillScreen(ST7735_BLACK);

  preferences.begin("netCreds", false); // Starts preferences, which handles storing in non-volatile memory

  setCreds(&tft, &preferences); // Sets network and API credentials in non-volatile memory

  apiPair = preferences.getString("id") + ":" + preferences.getString("key");           // These 3 lines create the base64 encoded pair
  int len = encode_base64((unsigned char *)apiPair.c_str(), apiPair.length(), encoded); // which is required to access the Trading212 API
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
      currentPage = MenuPageHandler(&tft, &previousPage);
      break;
    case SUMMARY_PAGE:
      summary = Summary(encodedPair, &WiFi);
      currentPage = SummaryPageHandler(&tft, &previousPage, summary);
      break;
    case POSITIONS_PAGE:
      // REMEMBER TO DEREFERENCE THE POSITIONS LINKED LIST WHEN EXITING THIS PAGE
      if (!positionsExists)
      {
        positionsJson = getPositions(encodedPair, &WiFi);
        positions = makePositions(positionsJson, &positionsExists);
      }
      currentPage = PositionsPageHandler(&tft, &WiFi, &previousPage, positions);
      if (positionsExists)
        freePositions(positions, &positionsExists);
      break;
    }
  }
}