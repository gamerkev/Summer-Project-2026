#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <keyboardFont.h>
#include <currencySymbols.h>
#include <Fonts/Picopixel.h>
#include <trading212Data.h>

#define TRADING21BLUE 0x053b
#define MENU_DARK_GREEN 0x0565
// Pins for the SPI interface
#define TFT_SCLK 7  // CLK
#define TFT_MOSI 11 // SDA
#define TFT_DC 2    // RS
#define TFT_RST 3   // RST
#define TFT_CS 12   // CS

class Adafruit_ST7735Ext : public Adafruit_ST7735
{ // Extend the display library to be able to cleanly add functionality
public:
  Adafruit_ST7735Ext(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : Adafruit_ST7735(cs, dc, mosi, sclk, rst) {} // Same constructor as the class it's extending

  void movingText(String toWrite, int y)
  {
    // int pixelLength = toWrite.length() * 6;                       // 6 pixels width each character
    int extraChars = toWrite.length() - 16;
    // int extraPixels = (ceil((pixelLength - 114)/6) * 6);    // chose the numbers from this line through trial and error
    fillRect(1, y - 12, 126, 12, ST7735_BLACK); // black out the line where the text will be
    setCursor(10, y);
    print(toWrite);
    fillRect(1, y - 12, 9, 12, ST7735_BLACK); // black out the edges of the text
    fillRect(118, y - 12, 9, 12, ST7735_BLACK);
    drawRect(0, 0, 128, 160, TRADING21BLUE);
    Serial.println(toWrite.length() + " long, need" + String(extraChars * 8) + " more chars");
    if (extraChars > 0)
    {
      delay(1000); // time to read the first part of the text
    }
    else
    {
      delay(500);
    }
    // THE MAX LENGTH OF TEXT THAT IS PADDED BY 9 PIXELS EITHER SIDE IS 18 CHARACTERS
    for (int i = 0; i < (extraChars * 8) + 1; i = i + 2)
    {
      fillRect(1, y - 12, 128, 12, ST7735_BLACK);
      setCursor(10 - i, y); // move 2 pixel to the left
      print(toWrite);
      fillRect(1, y - 12, 9, 12, ST7735_BLACK);
      fillRect(118, y - 12, 9, 12, ST7735_BLACK);
      drawRect(0, 0, 128, 160, TRADING21BLUE);
      delay(50);
    }
  }

  void logo()
  {
    fillScreen(ST7735_BLACK);
    fillTriangle(24, 109, 47, 109, 64, 61, TRADING21BLUE);
    delay(200);
    fillTriangle(65, 61, 82, 109, 105, 109, TRADING21BLUE);
    delay(200);
    fillTriangle(105, 109, 75, 27, 65, 61, TRADING21BLUE);
    delay(200);
    fillTriangle(24, 109, 64, 61, 54, 27, TRADING21BLUE);
    delay(200);
    fillTriangle(54, 27, 64, 61, 75, 27, TRADING21BLUE);
    delay(200);
    fillTriangle(75, 27, 65, 61, 54, 27, TRADING21BLUE);
    delay(200);
    setTextSize(2);
    setCursor(4, 125);
    println("Trading212");
    setTextSize(1);
    delay(3000);
  }

  void setFontKeepSize(const GFXfont *f)
  {
    if (f)
    { // Font struct pointer passed in?
      if (!gfxFont)
      { // And no current font struct?
        // Switching from classic to new font behavior.
        // Move cursor pos down 6 pixels so it's on baseline.
        cursor_y += 6;
      }
    }
    else if (gfxFont)
    { // NULL passed.  Current font struct defined?
      // Switching from new to classic font behavior.
      // Move cursor pos up 6 pixels so it's at top-left of char.
      cursor_y -= 7;
    }
    gfxFont = (GFXfont *)f;
  }

  void printMoney(currency curr, float amount)
  {
    setFontKeepSize(&CurrencySymbols);
    print(curr);
    setFontKeepSize(NULL);
    println(amount);
  }

  void printCentreLeftAlign(String toPrint, int y){
    String printing;
    setCursor(5, y);
    while(toPrint.length()>19){
      printing = toPrint.substring(0, 19);
      if (printing.endsWith(" ") or toPrint[19] == ' '){
        printing = printing.substring(0, 19);
        toPrint = toPrint.substring(19);
      } else{
        toPrint = toPrint.substring(printing.lastIndexOf(' '));
        printing = printing.substring(0, printing.lastIndexOf(' '));
      }
      if (printing[0] == ' '){
        printing = printing.substring(1);
      }
      println(printing);
      setCursor(5, cursor_y);
    }
    if (!toPrint.isEmpty()){
      if (toPrint[0] == ' ') println(toPrint.substring(1)); else println(toPrint);
    }
  }

  void printUnderlineDefaultFont(String toPrint, uint16_t colour){
    uint16_t oldColour = textcolor;
    drawLine(cursor_x, cursor_y+8, cursor_x+(toPrint.length()*6), cursor_y+8, colour);
    setTextColor(colour);
    println(toPrint);
    setTextColor(oldColour);
  }

  void printSummary(Summary aSummary){
    uint16_t oldColour = textcolor;
    setCursor(5, 5);
    printUnderlineDefaultFont("Summary", TRADING21BLUE);
    printCentreLeftAlign("Available to trade:", 20);
    setCursor(5, cursor_y+1);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getAvailableToTrade());
    printCentreLeftAlign("Total investments value:", 45);
    setCursor(41, cursor_y-7);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getTotalVal());
    printCentreLeftAlign("Paid for current shares:", 70);
    setCursor(47, cursor_y-7);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getTotalCost());
    printCentreLeftAlign("Unrealised profit:", 95);
    setCursor(5, cursor_y+1);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getUnrealisedProfit());
    printCentreLeftAlign("Reserved for orders:", 120);
    setCursor(47, cursor_y-7);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getReservedForOrders());
    fillRoundRect(5, 145, 28, 10, 1, MENU_DARK_GREEN);
    setCursor(7, 146);
    print("Menu");
    fillRoundRect(97, 145, 28, 10, 1, MENU_DARK_GREEN);
    setCursor(99, 146);
    print("Pies");
    setTextColor(oldColour);
  }
};