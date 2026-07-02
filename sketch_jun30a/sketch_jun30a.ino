#include <Adafruit_GFX.h>     
#include <Adafruit_ST7735.h>
#include <BleKeyboard.h>

#define TFT_CS  7
#define TFT_RST 21
#define TFT_DC  1
#define TFT_MOSI  6  // SDA // HW MOSI
#define TFT_SCLK  4  // SCL // HW SCLK

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
BleKeyboard bleKeyboard;

void setup(void) {
  Serial.begin(9600);
  bleKeyboard.end();
  bleKeyboard.begin();
  //tft.initB();
  tft.initR(INITR_BLACKTAB); // Init ST7735S chip, black tab
  //tft.initR(INITR_GREENTAB); // Init ST7735S chip, green tab
  //tft.setSPISpeed(27000000);

  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  // tft.setTextColor(ST77XX_WHITE);
  // tft.setTextSize(2);
  // tft.setCursor(15, 50);
  // tft.println("Makerguides");
  tft.drawRect(0, 0, 160, 128, ST77XX_YELLOW);
}

void loop() {
  if(bleKeyboard.isConnected()){
    // tft.fillScreen(ST7735_BLACK);
    tft.setCursor(15, 50);
    tft.setTextColor(ST7735_CYAN);
    tft.setTextSize(5);
    tft.println("Connected");
    delay(500);
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    // delay(500);
    // bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    Serial.println("Connected");
  } else{
    // tft.fillScreen(ST7735_BLACK);
    tft.drawRect(0, 0, 160, 128, ST7735_YELLOW);
    Serial.println("Disconnected");
  }
}