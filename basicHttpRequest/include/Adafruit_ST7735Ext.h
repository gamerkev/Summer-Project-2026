#include <Adafruit_ST7735.h>
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

typedef enum
{
  SUMMARY = 0,
  OPEN_POSITIONS = 1,
} MenuSelection;

typedef enum
{
  MENU_SUMMARY = 0,
  PIES = 1,
} SummarySelection;

typedef enum
{
  BUY = 0,
  SELL = 1,
  BACK = 2,
} PositionSelection;

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

  void printCentreLeftAlign(String toPrint, int y)
  {
    String printing;
    setCursor(5, y);
    while (toPrint.length() > 19)
    {
      printing = toPrint.substring(0, 19);
      if (printing.endsWith(" ") or toPrint[19] == ' ')
      {
        printing = printing.substring(0, 19);
        toPrint = toPrint.substring(19);
      }
      else
      {
        toPrint = toPrint.substring(printing.lastIndexOf(' '));
        printing = printing.substring(0, printing.lastIndexOf(' '));
      }
      if (printing[0] == ' ')
      {
        printing = printing.substring(1);
      }
      println(printing);
      setCursor(5, cursor_y);
    }
    if (!toPrint.isEmpty())
    {
      if (toPrint[0] == ' ')
        println(toPrint.substring(1));
      else
        println(toPrint);
    }
  }

  void printUnderlineDefaultFont(String toPrint, uint16_t colour)
  {
    uint16_t oldColour = textcolor;
    drawLine(cursor_x, cursor_y + 8, cursor_x + (toPrint.length() * 6), cursor_y + 8, colour);
    setTextColor(colour);
    println(toPrint);
    setTextColor(oldColour);
  }

  void printCentered(String toPrint)
  {
    cursor_x = (128 - (toPrint.length() * 6)) / 2;
    println(toPrint);
  }

  void printSummary(Summary aSummary, SummarySelection summarySelection)
  {
    uint16_t oldColour = textcolor;
    fillScreen(ST7735_BLACK);
    setCursor(5, 5);
    printUnderlineDefaultFont("Summary", TRADING21BLUE);
    printCentreLeftAlign("Available to trade:", 20);
    setCursor(5, cursor_y + 1);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getAvailableToTrade());
    printCentreLeftAlign("Total investments value:", 45);
    setCursor(41, cursor_y - 7);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getTotalVal());
    printCentreLeftAlign("Paid for current shares:", 70);
    setCursor(47, cursor_y - 7);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getTotalCost());
    printCentreLeftAlign("Unrealised profit:", 95);
    setCursor(5, cursor_y + 1);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getUnrealisedProfit());
    printCentreLeftAlign("Reserved for orders:", 120);
    setCursor(47, cursor_y - 7);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getReservedForOrders());
    switch (summarySelection)
    {
    case MENU_SUMMARY:
      fillRoundRect(5, 145, 28, 12, 1, TRADING21BLUE);
      setCursor(7, 147);
      print("Menu");
      drawRoundRect(97, 145, 28, 12, 1, TRADING21BLUE);
      setCursor(99, 147);
      setTextColor(TRADING21BLUE);
      print("Pies");
      setTextColor(oldColour);
      break;
    case PIES:
      drawRoundRect(5, 145, 28, 12, 1, TRADING21BLUE);
      setCursor(7, 147);
      setTextColor(TRADING21BLUE);
      print("Menu");
      setTextColor(oldColour);
      fillRoundRect(97, 145, 28, 12, 1, TRADING21BLUE);
      setCursor(99, 147);
      print("Pies");
      break;
    }
  }

  void printPageNum(int pageNum, int totalPages)
  {
    setCursor(0, 150);
    printCentered("< " + String(pageNum) + "/" + String(totalPages) + " >");
  }

  void printPositions(Positions *positions, int totalCount, int select)
  {
    fillScreen(ST7735_BLACK);
    uint16_t oldColour = textcolor;
    setCursor(5, 5);
    printUnderlineDefaultFont("Positions", TRADING21BLUE);
    int firstPosY = 20;
    int count; // Will display count-positions
    count = positions->count > 7 ? 7 : positions->count;
    Positions currentPosition = *positions;
    for (int i = 0; i < count - 1; i++)
    {
      if (currentPosition.currentPos.getUnrealisedProfit() >= 0)
        setTextColor(ST7735_GREEN);
      else
        setTextColor(ST7735_RED);
      printCentreLeftAlign(currentPosition.currentPos.getName(), firstPosY + (i * 17));
      setCursor(5, cursor_y + 1);
      printMoney(currencySymbol(currentPosition.currentPos.getWalletCurrency()), currentPosition.currentPos.getUnrealisedProfit());
      currentPosition = *currentPosition.nextPos;
    }

    if (currentPosition.currentPos.getUnrealisedProfit() >= 0)
      setTextColor(ST7735_GREEN);
    else
      setTextColor(ST7735_RED);
    printCentreLeftAlign(currentPosition.currentPos.getName(), firstPosY + ((count - 1) * 17));
    setCursor(5, cursor_y + 1);
    printMoney(currencySymbol(currentPosition.currentPos.getWalletCurrency()), currentPosition.currentPos.getUnrealisedProfit());
    switch (select)
    {
    case 7:
      fillRoundRect(5, 145, 28, 12, 1, TRADING21BLUE);
      setTextColor(oldColour);
      setCursor(7, 147);
      print("Menu");
      printPageNum(((totalCount - positions->count) / 7 + 1), (totalCount / 7) + 1);
      break;
    default:
      drawRoundRect(5, 145, 28, 12, 1, TRADING21BLUE);
      setCursor(7, 147);
      setTextColor(TRADING21BLUE);
      print("Menu");
      setTextColor(oldColour);
      printPageNum(((totalCount - positions->count) / 7 + 1), (totalCount / 7) + 1);
      drawRect(1, firstPosY + 1 + (select * 17), 2, 13, ST7735_CYAN);
    }
  }

  void printPosition(Position position, int positionCount, int totalCount, PositionSelection select)
  {
    uint16_t oldColour = textcolor;
    fillScreen(ST7735_BLACK);
    setCursor(5, 5);
    printUnderlineDefaultFont(position.getName(), TRADING21BLUE);
    int firstPosY = 20;
    printCentreLeftAlign("Paid per share:", firstPosY);
    setCursor(5, cursor_y + 1);
    printMoney(currencySymbol(position.getInstrumentCurrency()), position.getPaidPerShare());
    printCentreLeftAlign("Current share value: ", firstPosY + 20);
    setCursor(42, cursor_y - 7);
    if (position.getCurrShareVal() - position.getPaidPerShare() >= 0)
      setTextColor(ST7735_GREEN);
    else
      setTextColor(ST7735_RED);
    printMoney(currencySymbol(position.getInstrumentCurrency()), position.getCurrShareVal());
    setTextColor(oldColour);
    printCentreLeftAlign("Unrealised profit:", firstPosY + 40);
    if (position.getUnrealisedProfit() >= 0)
      setTextColor(ST7735_GREEN);
    else
      setTextColor(ST7735_RED);
    setCursor(5, cursor_y + 1);
    printMoney(currencySymbol(position.getWalletCurrency()), position.getUnrealisedProfit());
    setTextColor(oldColour);
    printCentreLeftAlign("Shares available:" + String(position.getSharesAvail()), firstPosY + 60);
    drawRoundRect(5, 145, 28, 12, 1, TRADING21BLUE);
    setCursor(7, 147);
    setTextColor(TRADING21BLUE);
    print("Back");
    drawRoundRect(5, 105, 22, 12, 1, ST7735_GREEN);
    setTextColor(ST7735_GREEN);
    setCursor(7, 107);
    print("Buy");
    drawRoundRect(5, 120, 28, 12, 1, ST7735_RED);
    setTextColor(ST7735_RED);
    setCursor(7, 122);
    print("Sell");
    switch (select)
    {
    case BUY:
      fillRoundRect(5, 105, 22, 12, 1, ST7735_GREEN);
      setTextColor(ST7735_BLACK);
      setCursor(7, 107);
      print("Buy");
      break;
    case SELL:
      fillRoundRect(5, 120, 28, 12, 1, ST7735_RED);
      setTextColor(oldColour);
      setCursor(7, 122);
      print("Sell");
      break;
    case BACK:
      fillRoundRect(5, 145, 28, 12, 1, TRADING21BLUE);
      setCursor(7, 147);
      setTextColor(oldColour);
      print("Back");
      break;
    }
    setTextColor(oldColour);
    printPageNum(totalCount - positionCount + 1, totalCount);
  }

  void printMenu(MenuSelection select)
  {
    fillScreen(ST7735_BLACK);
    setTextSize(2);
    setCursor(40, 20);
    println("MENU");
    setTextSize(1);
    switch (select)
    {
    case SUMMARY:
      fillRoundRect(40, 48, 46, 12, 2, TRADING21BLUE);
      setCursor(0, 50);
      printCentered("Summary");
      drawRoundRect(34, 68, 58, 12, 2, TRADING21BLUE);
      setCursor(0, 70);
      printCentered("Positions");
      break;
    case OPEN_POSITIONS:
      drawRoundRect(40, 48, 46, 12, 2, TRADING21BLUE);
      setCursor(0, 50);
      printCentered("Summary");
      fillRoundRect(34, 68, 58, 12, 2, TRADING21BLUE);
      setCursor(0, 70);
      printCentered("Positions");
      break;
    default:
      drawRoundRect(40, 48, 46, 12, 2, TRADING21BLUE);
      setCursor(0, 50);
      printCentered("Summary");
      drawRoundRect(34, 68, 58, 12, 2, TRADING21BLUE);
      setCursor(0, 70);
      printCentered("Positions");
    }
  }
};