#include <Adafruit_ST7735.h>
#include <trading212Data.h>
#include <keyboardFont.h>
#include <currencySymbols.h>

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

typedef enum
{
  LONG_HOURS = 0,
  CANCEL_ORDER = 1,
  CONFIRM_ORDER = 2,
} BuySellConfirmationSelection;

class Adafruit_ST7735Ext : public Adafruit_ST7735
{ // Extend the display library to be able to cleanly add functionality
public:
  Adafruit_ST7735Ext(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : Adafruit_ST7735(cs, dc, mosi, sclk, rst) {} // Same constructor as the class it's extending

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

  void drawTickSmall(int x, int y, uint16_t colour)
  {
    drawLine(x, y, x + 2, y + 2, colour);
    drawLine(x + 2, y + 2, x + 7, y - 3, colour);
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

  void printMoney(Currency curr, float amount)
  {
    setFontKeepSize(&CurrencySymbols);
    print(curr);
    setFontKeepSize(NULL);
    println(amount);
  }

  void printCentreLeftAlign(String toPrint, int y)
  {
    String printing;
    setCursor(5, y);              // Pad by 5 pixlels
    while (toPrint.length() > 19) // Check that the text isn't too long to fit on one line
    {
      printing = toPrint.substring(0, 19);              // Print as much text as can be printed on one line
      if (printing.endsWith(" ") or toPrint[19] == ' ') // Don't print trailing spaces
      {
        printing = printing.substring(0, 19); // What will be printed now
        toPrint = toPrint.substring(19);      // What is left to be printed
      }
      else // To avoid splitting words onto different lines
      {
        toPrint = toPrint.substring(printing.lastIndexOf(' '));
        printing = printing.substring(0, printing.lastIndexOf(' '));
      }
      if (printing[0] == ' ') // Again to avoid printing trailing spaces
      {
        printing = printing.substring(1);
      }
      println(printing);
      setCursor(5, cursor_y); // Again pad by 5 pixels
    }
    if (!toPrint.isEmpty()) // Print what is left of the string if it wasn't a perfect multiple
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

  void printSummary(Summary aSummary, SummarySelection select)
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
    if (aSummary.getUnrealisedProfit() < 0)
      setTextColor(ST7735_GREEN);
    else
      setTextColor(ST7735_RED);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getUnrealisedProfit());
    setTextColor(oldColour);
    printCentreLeftAlign("Reserved for orders:", 120);
    setCursor(47, cursor_y - 7);
    printMoney(currencySymbol(aSummary.getCurr()), aSummary.getReservedForOrders());
    switch (select)
    {
    case MENU_SUMMARY: // If the summary button is selected
      fillRoundRect(5, 145, 28, 12, 1, TRADING21BLUE);
      setCursor(7, 147);
      print("Menu");
      drawRoundRect(97, 145, 28, 12, 1, TRADING21BLUE);
      setCursor(99, 147);
      setTextColor(TRADING21BLUE);
      print("Pies");
      setTextColor(oldColour);
      break;
    case PIES: // If the pies button is selected
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

  void printPositions(Positions *aPositions, int totalCount, int select)
  {
    fillScreen(ST7735_BLACK);
    uint16_t oldColour = textcolor;
    setCursor(5, 5);
    printUnderlineDefaultFont("Positions", TRADING21BLUE);
    int firstPosY = 20;
    int count;
    count = aPositions->count > 7 ? 7 : aPositions->count; // How many positions will be printed on the display
    Positions currentPosition = *aPositions;
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
    case 7: // If the menu button is selected
      fillRoundRect(5, 145, 28, 12, 1, TRADING21BLUE);
      setTextColor(oldColour);
      setCursor(7, 147);
      print("Menu");
      printPageNum(((totalCount - aPositions->count) / 7 + 1), (totalCount / 7) + 1);
      break;
    default: // If any position is selected
      drawRoundRect(5, 145, 28, 12, 1, TRADING21BLUE);
      setCursor(7, 147);
      setTextColor(TRADING21BLUE);
      print("Menu");
      setTextColor(oldColour);
      printPageNum(((totalCount - aPositions->count) / 7 + 1), (totalCount / 7) + 1);
      drawRect(1, firstPosY + 1 + (select * 17), 2, 13, ST7735_CYAN);
    }
  }

  void printPosition(Position aPosition, int positionCount, int totalCount, PositionSelection select)
  {
    uint16_t oldColour = textcolor;
    fillScreen(ST7735_BLACK);
    setCursor(5, 5);
    printUnderlineDefaultFont(aPosition.getName(), TRADING21BLUE);
    int firstPosY = 20;
    printCentreLeftAlign("Paid per share:", firstPosY);
    setCursor(5, cursor_y + 1);
    printMoney(currencySymbol(aPosition.getInstrumentCurrency()), aPosition.getPaidPerShare());
    printCentreLeftAlign("Current share value: ", firstPosY + 20);
    setCursor(42, cursor_y - 7);
    if (aPosition.getCurrShareVal() - aPosition.getPaidPerShare() >= 0)
      setTextColor(ST7735_GREEN);
    else
      setTextColor(ST7735_RED);
    printMoney(currencySymbol(aPosition.getInstrumentCurrency()), aPosition.getCurrShareVal());
    setTextColor(oldColour);
    printCentreLeftAlign("Unrealised profit:", firstPosY + 40);
    if (aPosition.getUnrealisedProfit() >= 0)
      setTextColor(ST7735_GREEN);
    else
      setTextColor(ST7735_RED);
    setCursor(5, cursor_y + 1);
    printMoney(currencySymbol(aPosition.getWalletCurrency()), aPosition.getUnrealisedProfit());
    setTextColor(oldColour);
    printCentreLeftAlign("Shares available:" + String(aPosition.getSharesAvail()), firstPosY + 60);
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
    case BUY: // If the buy button is selected
      fillRoundRect(5, 105, 22, 12, 1, ST7735_GREEN);
      setTextColor(ST7735_BLACK);
      setCursor(7, 107);
      print("Buy");
      break;
    case SELL: // If the sell button is selected
      fillRoundRect(5, 120, 28, 12, 1, ST7735_RED);
      setTextColor(oldColour);
      setCursor(7, 122);
      print("Sell");
      break;
    case BACK: // If the back button is selected
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
    case SUMMARY: // If the summary button is selected
      fillRoundRect(40, 48, 46, 12, 2, TRADING21BLUE);
      setCursor(0, 50);
      printCentered("Summary");
      drawRoundRect(34, 68, 58, 12, 2, TRADING21BLUE);
      setCursor(0, 70);
      printCentered("Positions");
      break;
    case OPEN_POSITIONS: // If the open positions button is selected
      drawRoundRect(40, 48, 46, 12, 2, TRADING21BLUE);
      setCursor(0, 50);
      printCentered("Summary");
      fillRoundRect(34, 68, 58, 12, 2, TRADING21BLUE);
      setCursor(0, 70);
      printCentered("Positions");
      break;
    }
  }

  void printBuySellMenu(bool buy)
  {
    fillRect(16, 20, 97, 121, ST7735_BLACK);
    drawRect(16, 20, 97, 121, TRADING21BLUE);
    switch (buy)
    {
    case true: // If the user is in the buy menu
      setCursor(20, 24);
      printUnderlineDefaultFont("Buy", ST7735_GREEN);
      break;
    case false: // If the user is in the sell menu
      setCursor(20, 24);
      printUnderlineDefaultFont("Sell", ST7735_RED);
      break;
    }
  }

  void printBuySellConfirmation(String posName, bool buy, bool quantity, float amount, BuySellConfirmationSelection select, bool longHours = false)
  {
    uint16_t oldColour = textcolor;
    posName.toUpperCase(); // Capitalise to draw attenntion to it
    switch (buy)
    {
    case true: // If the user is buying
      drawRect(0, 0, 128, 160, ST7735_GREEN);
      switch (quantity)
      {
      case true: // If the user has placed their order as a quantity of shares
        printCentreLeftAlign("Are you sure that you want to BUY " + String(amount) + " shares of " + posName + "?", 10);
        break;
      case false: // If the user has placed their order in terms of an amoount money
        printCentreLeftAlign("Are you sure that you want to BUY " + String(amount) + " GBP of " + posName + "?", 10);
        break;
      }
      break;
    case false: // If the user is selling
      drawRect(0, 0, 128, 160, ST7735_RED);
      switch (quantity)
      {
      case true: // If the user has placed their order as a quantity of shares
        printCentreLeftAlign("Are you sure that you want to SELL " + String(amount) + " shares of " + posName + "?", 10);
        break;
      case false: // If the user has placed their order in terms of an amoount money
        printCentreLeftAlign("Are you sure that you want to SELL " + String(amount) + " GBP of " + posName + "?", 10);
        break;
      }
      break;
    }

    switch (select)
    {
    case LONG_HOURS: // If the user has selected the long hours button
      setTextColor(TRADING21BLUE);
      setCursor(39, 120);
      print("Long hours");

      fillRoundRect(13, 133, 45, 11, 1, ST7735_BLACK);
      setTextColor(ST7735_GREEN);
      setCursor(15, 135);
      print("CONFIRM");
      drawRoundRect(13, 133, 45, 11, 1, ST7735_GREEN);

      fillRoundRect(75, 133, 39, 11, 1, ST7735_BLACK);
      setTextColor(ST7735_RED);
      setCursor(77, 135);
      print("CANCEL");
      drawRoundRect(75, 133, 39, 11, 1, ST7735_RED);

      break;

    case CONFIRM_ORDER: // If the user has selected the confirm order button
      setTextColor(ST7735_WHITE);
      setCursor(39, 120);
      print("Long hours");

      fillRoundRect(13, 133, 45, 11, 1, ST7735_GREEN);
      setTextColor(ST7735_BLACK);
      setCursor(15, 135);
      print("CONFIRM");

      fillRoundRect(75, 133, 39, 11, 1, ST7735_BLACK);
      setTextColor(ST7735_RED);
      setCursor(77, 135);
      print("CANCEL");
      drawRoundRect(75, 133, 39, 11, 1, ST7735_RED);

      break;

    case CANCEL_ORDER: // If the user has selected the cancel order button
      setTextColor(ST7735_WHITE);
      setCursor(39, 120);
      print("Long hours");

      fillRoundRect(13, 133, 45, 11, 1, ST7735_BLACK);
      setTextColor(ST7735_GREEN);
      setCursor(15, 135);
      print("CONFIRM");
      drawRoundRect(13, 133, 45, 11, 1, ST7735_GREEN);

      fillRoundRect(75, 133, 39, 11, 1, ST7735_RED);
      setTextColor(ST7735_WHITE);
      setCursor(77, 135);
      print("CANCEL");

      break;
    }
    if (longHours) // If the long hours box has been ticked
    {
      drawTickSmall(31, 122, ST7735_WHITE);
      drawRect(30, 120, 7, 6, ST7735_WHITE);
    }
    else // If it has not yet been ticked
    {
      drawTickSmall(31, 122, ST7735_BLACK);
      drawRect(30, 120, 7, 6, ST7735_WHITE);
    }
    setTextColor(oldColour);
  }

  void printConnectionRefused()
  {
    fillRoundRect(14, 50, 100, 34, 3, ST7735_BLACK);
    drawRoundRect(14, 50, 100, 34, 3, TRADING21BLUE);
    setCursor(34, 55);
    println("Connection");
    cursor_x = 19;
    println("refused, please");
    cursor_x = 34;
    print("try again.");
  }
};