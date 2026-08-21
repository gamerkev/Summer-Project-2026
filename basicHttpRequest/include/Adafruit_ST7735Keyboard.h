#include <Adafruit_ST7735Ext.h>
#include <Fonts/Picopixel.h>

#define SHIFT '/'
#define BACKSPACE '*'
#define ENTER '~'
#define QUANTITY 'q'
#define VALUE 'v'
#define LIMIT 'l'

typedef enum
{
  LEFT = 0,
  UP = 1,
  RIGHT = 2,
  DOWN = 3
} KeyboardDirection;

class Adafruit_ST7735Keyboard : public Adafruit_ST7735Ext
{
public:
  Adafruit_ST7735Keyboard(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : Adafruit_ST7735Ext(cs, dc, mosi, sclk, rst) {}

  String takeInput(int x, int y)
  {
    GFXfont *oldFond = gfxFont;
    String in;
    String tftReturn;
    String toReturn;
    bool done = false;
    while (!done)
    {
      while (Serial.available())
      {
        in = Serial.readString();
        if (in == "a")
        {
          changeLetter(LEFT);
        }
        else if (in == "w")
        {
          changeLetter(UP);
        }
        else if (in == "d")
        {
          changeLetter(RIGHT);
        }
        else if (in == "s")
        {
          changeLetter(DOWN);
        }
        else if (in == "l")
        {
          tftReturn = inputKey(toReturn);
          if (!tftReturn.isEmpty() or getCurrentLetter() == BACKSPACE)
          {
            toReturn = tftReturn;
          }
          setFont(&Keyboard);
          setCursor(x, y);
          fillRect(1, y - 12, 126, 12, ST7735_BLACK);
          if (toReturn.length() < 14)
          {
            print(toReturn);
          }
          else
          {
            print(toReturn.substring(toReturn.length() - 14, toReturn.length() - 1));
          }
          if (getCurrentLetter() == ENTER)
          {
            done = true;
          }
          setFont();
        }
      }
    }
    return toReturn;
  }

  void drawBackspace(int x, int y, uint16_t colour)
  {
    drawLine(x, y, x + 7, y, colour);
    drawLine(x, y, x + 2, y + 2, colour);
    drawLine(x, y, x + 2, y - 2, colour);
  }

  void drawEnter(int x, int y, uint16_t colour)
  {
    drawLine(x + 10, y - 12, x + 10, y, colour);
    drawLine(x, y, x + 10, y, colour);
    drawLine(x, y, x + 2, y + 2, colour);
    drawLine(x, y, x + 2, y - 2, colour);
  }

  void drawTick(int x, int y, uint16_t colour)
  {
    drawLine(x, y, x + 3, y + 3, colour);
    drawLine(x + 3, y + 3, x + 9, y - 3, colour);
  }

  void drawTickSmall(int x, int y, uint16_t colour)
  {
    drawLine(x, y, x + 2, y + 2, colour);
    drawLine(x + 2, y + 2, x + 7, y - 3, colour);
  }

  void drawCross(int x, int y, uint16_t colour)
  {
    drawLine(x, y, x + 5, y + 5, colour);
    drawLine(x, y + 5, x + 5, y, colour);
  }

  void putKeyboard(int height, bool capital)
  {
    GFXfont *oldFont = gfxFont;
    uint16_t oldColour = textcolor;
    setTextColor(ST7735_WHITE);
    keyboardHeight = height;
    caps = capital;
    setFont(&Keyboard);
    fillRect(1, keyboardHeight - 13, 126, 61, ST7735_BLACK);
    // Top row
    setCursor(11, keyboardHeight);
    print("1");
    setCursor(21, keyboardHeight);
    print("2");
    setCursor(31, keyboardHeight);
    print("3");
    setCursor(41, keyboardHeight);
    print("4");
    setCursor(51, keyboardHeight);
    print("5");
    setCursor(61, keyboardHeight);
    print("6");
    setCursor(71, keyboardHeight);
    print("7");
    setCursor(81, keyboardHeight);
    print("8");
    setCursor(91, keyboardHeight);
    print("9");
    setCursor(101, keyboardHeight);
    print("0");
    setCursor(111, keyboardHeight);
    print("-");
    if (!capital)
    {
      // Second row
      setCursor(16, keyboardHeight + 12);
      print("q");
      setCursor(26, keyboardHeight + 12);
      print("w");
      setCursor(36, keyboardHeight + 12);
      print("e");
      setCursor(46, keyboardHeight + 12);
      print("r");
      setCursor(56, keyboardHeight + 12);
      print("t");
      setCursor(66, keyboardHeight + 12);
      print("y");
      setCursor(76, keyboardHeight + 12);
      print("u");
      setCursor(86, keyboardHeight + 12);
      print("i");
      setCursor(96, keyboardHeight + 12);
      print("o");
      setCursor(106, keyboardHeight + 12);
      print("p");
      // Third row
      setCursor(21, keyboardHeight + 24);
      print("a");
      setCursor(31, keyboardHeight + 24);
      print("s");
      setCursor(41, keyboardHeight + 24);
      print("d");
      setCursor(51, keyboardHeight + 24);
      print("f");
      setCursor(61, keyboardHeight + 24);
      print("g");
      setCursor(71, keyboardHeight + 24);
      print("h");
      setCursor(81, keyboardHeight + 24);
      print("j");
      setCursor(91, keyboardHeight + 24);
      print("k");
      setCursor(101, keyboardHeight + 24);
      print("l");
      // final row
      setCursor(31, keyboardHeight + 36);
      print("z");
      setCursor(41, keyboardHeight + 36);
      print("x");
      setCursor(51, keyboardHeight + 36);
      print("c");
      setCursor(61, keyboardHeight + 36);
      print("v");
      setCursor(71, keyboardHeight + 36);
      print("b");
      setCursor(81, keyboardHeight + 36);
      print("n");
      setCursor(91, keyboardHeight + 36);
      print("m");
    }
    else
    {
      // Second row
      setCursor(16, keyboardHeight + 12);
      print("Q");
      setCursor(26, keyboardHeight + 12);
      print("W");
      setCursor(36, keyboardHeight + 12);
      print("E");
      setCursor(46, keyboardHeight + 12);
      print("R");
      setCursor(56, keyboardHeight + 12);
      print("T");
      setCursor(66, keyboardHeight + 12);
      print("Y");
      setCursor(76, keyboardHeight + 12);
      print("U");
      setCursor(86, keyboardHeight + 12);
      print("I");
      setCursor(96, keyboardHeight + 12);
      print("O");
      setCursor(106, keyboardHeight + 12);
      print("P");
      // Third row
      setCursor(21, keyboardHeight + 24);
      print("A");
      setCursor(31, keyboardHeight + 24);
      print("S");
      setCursor(41, keyboardHeight + 24);
      print("D");
      setCursor(51, keyboardHeight + 24);
      print("F");
      setCursor(61, keyboardHeight + 24);
      print("G");
      setCursor(71, keyboardHeight + 24);
      print("H");
      setCursor(81, keyboardHeight + 24);
      print("J");
      setCursor(91, keyboardHeight + 24);
      print("K");
      setCursor(101, keyboardHeight + 24);
      print("L");
      // final row
      setCursor(31, keyboardHeight + 36);
      print("Z");
      setCursor(41, keyboardHeight + 36);
      print("X");
      setCursor(51, keyboardHeight + 36);
      print("C");
      setCursor(61, keyboardHeight + 36);
      print("V");
      setCursor(71, keyboardHeight + 36);
      print("B");
      setCursor(81, keyboardHeight + 36);
      print("N");
      setCursor(91, keyboardHeight + 36);
      print("M");
    }
    // shift key
    setFont(&Picopixel);
    setCursor(5, keyboardHeight + 31);
    print("shift");
    // spacebar
    drawLine(39, keyboardHeight + 41, 84, keyboardHeight + 41, ST7735_WHITE);
    drawBackspace(6, keyboardHeight + 18, ST7735_WHITE);
    drawEnter(112, keyboardHeight + 18, ST7735_WHITE);
    selectKey(currentLetter);
    setFont(oldFont);
    setTextColor(oldColour);
  }

  void selectKey(char key)
  {
    GFXfont *oldFont = gfxFont;
    uint16_t oldColour = textcolor;
    setTextColor(TRADING21BLUE);
    setFont(&Keyboard);
    switch (key)
    {
    case '1':
      setCursor(11, keyboardHeight);
      print("1");
      break;
    case '2':
      setCursor(21, keyboardHeight);
      print("2");
      break;
    case '3':
      setCursor(31, keyboardHeight);
      print("3");
      break;
    case '4':
      setCursor(41, keyboardHeight);
      print("4");
      break;
    case '5':
      setCursor(51, keyboardHeight);
      print("5");
      break;
    case '6':
      setCursor(61, keyboardHeight);
      print("6");
      break;
    case '7':
      setCursor(71, keyboardHeight);
      print("7");
      break;
    case '8':
      setCursor(81, keyboardHeight);
      print("8");
      break;
    case '9':
      setCursor(91, keyboardHeight);
      print("9");
      break;
    case '0':
      setCursor(101, keyboardHeight);
      print("0");
      break;
    case '-':
      setCursor(111, keyboardHeight);
      print("-");
      break;
    case 'q':
      setCursor(16, keyboardHeight + 12);
      if (!caps)
        print("q");
      else
        print("Q");
      break;
    case 'w':
      setCursor(26, keyboardHeight + 12);
      if (!caps)
        print("w");
      else
        print("W");
      break;
    case 'e':
      setCursor(36, keyboardHeight + 12);
      if (!caps)
        print("e");
      else
        print("E");
      break;
    case 'r':
      setCursor(46, keyboardHeight + 12);
      if (!caps)
        print("r");
      else
        print("R");
      break;
    case 't':
      setCursor(56, keyboardHeight + 12);
      if (!caps)
        print("t");
      else
        print("T");
      break;
    case 'y':
      setCursor(66, keyboardHeight + 12);
      if (!caps)
        print("y");
      else
        print("Y");
      break;
    case 'u':
      setCursor(76, keyboardHeight + 12);
      if (!caps)
        print("u");
      else
        print("U");
      break;
    case 'i':
      setCursor(86, keyboardHeight + 12);
      if (!caps)
        print("i");
      else
        print("I");
      break;
    case 'o':
      setCursor(96, keyboardHeight + 12);
      if (!caps)
        print("o");
      else
        print("O");
      break;
    case 'p':
      setCursor(106, keyboardHeight + 12);
      if (!caps)
        print("p");
      else
        print("P");
      break;
    case 'a':
      setCursor(21, keyboardHeight + 24);
      if (!caps)
        print("a");
      else
        print("A");
      break;
    case 's':
      setCursor(31, keyboardHeight + 24);
      if (!caps)
        print("s");
      else
        print("S");
      break;
    case 'd':
      setCursor(41, keyboardHeight + 24);
      if (!caps)
        print("d");
      else
        print("D");
      break;
    case 'f':
      setCursor(51, keyboardHeight + 24);
      if (!caps)
        print("f");
      else
        print("F");
      break;
    case 'g':
      setCursor(61, keyboardHeight + 24);
      if (!caps)
        print("g");
      else
        print("G");
      break;
    case 'h':
      setCursor(71, keyboardHeight + 24);
      if (!caps)
        print("h");
      else
        print("H");
      break;
    case 'j':
      setCursor(81, keyboardHeight + 24);
      if (!caps)
        print("j");
      else
        print("J");
      break;
    case 'k':
      setCursor(91, keyboardHeight + 24);
      if (!caps)
        print("k");
      else
        print("K");
      break;
    case 'l':
      setCursor(101, keyboardHeight + 24);
      if (!caps)
        print("l");
      else
        print("L");
      break;
    case 'z':
      setCursor(31, keyboardHeight + 36);
      if (!caps)
        print("z");
      else
        print("Z");
      break;
    case 'x':
      setCursor(41, keyboardHeight + 36);
      if (!caps)
        print("x");
      else
        print("X");
      break;
    case 'c':
      setCursor(51, keyboardHeight + 36);
      if (!caps)
        print("c");
      else
        print("C");
      break;
    case 'v':
      setCursor(61, keyboardHeight + 36);
      if (!caps)
        print("v");
      else
        print("V");
      break;
    case 'b':
      setCursor(71, keyboardHeight + 36);
      if (!caps)
        print("b");
      else
        print("B");
      break;
    case 'n':
      setCursor(81, keyboardHeight + 36);
      if (!caps)
        print("n");
      else
        print("N");
      break;
    case 'm':
      setCursor(91, keyboardHeight + 36);
      if (!caps)
        print("m");
      else
        print("M");
      break;
    case SHIFT: // shift key
      setFont(&Picopixel);
      setCursor(5, keyboardHeight + 31);
      print("shift");
      break;
    case ' ': // space key
      drawLine(39, keyboardHeight + 41, 84, keyboardHeight + 41, TRADING21BLUE);
      break;
    case BACKSPACE:
      drawBackspace(6, keyboardHeight + 18, TRADING21BLUE);
      break;
    case ENTER:
      drawEnter(112, keyboardHeight + 18, TRADING21BLUE);
      break;
    }
    setFont(oldFont);
    setTextColor(oldColour);
  }

  void deselectKey(char key)
  {
    GFXfont *oldFont = gfxFont;
    uint16_t oldColour = textcolor;
    setTextColor(ST7735_WHITE);
    setFont(&Keyboard);
    switch (key)
    {
    case '1':
      setCursor(11, keyboardHeight);
      print("1");
      break;
    case '2':
      setCursor(21, keyboardHeight);
      print("2");
      break;
    case '3':
      setCursor(31, keyboardHeight);
      print("3");
      break;
    case '4':
      setCursor(41, keyboardHeight);
      print("4");
      break;
    case '5':
      setCursor(51, keyboardHeight);
      print("5");
      break;
    case '6':
      setCursor(61, keyboardHeight);
      print("6");
      break;
    case '7':
      setCursor(71, keyboardHeight);
      print("7");
      break;
    case '8':
      setCursor(81, keyboardHeight);
      print("8");
      break;
    case '9':
      setCursor(91, keyboardHeight);
      print("9");
      break;
    case '0':
      setCursor(101, keyboardHeight);
      print("0");
      break;
    case '-':
      setCursor(111, keyboardHeight);
      print("-");
      break;
    case 'q':
      setCursor(16, keyboardHeight + 12);
      if (!caps)
        print("q");
      else
        print("Q");
      break;
    case 'w':
      setCursor(26, keyboardHeight + 12);
      if (!caps)
        print("w");
      else
        print("W");
      break;
    case 'e':
      setCursor(36, keyboardHeight + 12);
      if (!caps)
        print("e");
      else
        print("E");
      break;
    case 'r':
      setCursor(46, keyboardHeight + 12);
      if (!caps)
        print("r");
      else
        print("R");
      break;
    case 't':
      setCursor(56, keyboardHeight + 12);
      if (!caps)
        print("t");
      else
        print("T");
      break;
    case 'y':
      setCursor(66, keyboardHeight + 12);
      if (!caps)
        print("y");
      else
        print("Y");
      break;
    case 'u':
      setCursor(76, keyboardHeight + 12);
      if (!caps)
        print("u");
      else
        print("U");
      break;
    case 'i':
      setCursor(86, keyboardHeight + 12);
      if (!caps)
        print("i");
      else
        print("I");
      break;
    case 'o':
      setCursor(96, keyboardHeight + 12);
      if (!caps)
        print("o");
      else
        print("O");
      break;
    case 'p':
      setCursor(106, keyboardHeight + 12);
      if (!caps)
        print("p");
      else
        print("P");
      break;
    case 'a':
      setCursor(21, keyboardHeight + 24);
      if (!caps)
        print("a");
      else
        print("A");
      break;
    case 's':
      setCursor(31, keyboardHeight + 24);
      if (!caps)
        print("s");
      else
        print("S");
      break;
    case 'd':
      setCursor(41, keyboardHeight + 24);
      if (!caps)
        print("d");
      else
        print("D");
      break;
    case 'f':
      setCursor(51, keyboardHeight + 24);
      if (!caps)
        print("f");
      else
        print("F");
      break;
    case 'g':
      setCursor(61, keyboardHeight + 24);
      if (!caps)
        print("g");
      else
        print("G");
      break;
    case 'h':
      setCursor(71, keyboardHeight + 24);
      if (!caps)
        print("h");
      else
        print("H");
      break;
    case 'j':
      setCursor(81, keyboardHeight + 24);
      if (!caps)
        print("j");
      else
        print("J");
      break;
    case 'k':
      setCursor(91, keyboardHeight + 24);
      if (!caps)
        print("k");
      else
        print("K");
      break;
    case 'l':
      setCursor(101, keyboardHeight + 24);
      if (!caps)
        print("l");
      else
        print("L");
      break;
    case 'z':
      setCursor(31, keyboardHeight + 36);
      if (!caps)
        print("z");
      else
        print("Z");
      break;
    case 'x':
      setCursor(41, keyboardHeight + 36);
      if (!caps)
        print("x");
      else
        print("X");
      break;
    case 'c':
      setCursor(51, keyboardHeight + 36);
      if (!caps)
        print("c");
      else
        print("C");
      break;
    case 'v':
      setCursor(61, keyboardHeight + 36);
      if (!caps)
        print("v");
      else
        print("V");
      break;
    case 'b':
      setCursor(71, keyboardHeight + 36);
      if (!caps)
        print("b");
      else
        print("B");
      break;
    case 'n':
      setCursor(81, keyboardHeight + 36);
      if (!caps)
        print("n");
      else
        print("N");
      break;
    case 'm':
      setCursor(91, keyboardHeight + 36);
      if (!caps)
        print("m");
      else
        print("M");
      break;
    case SHIFT: // shift key
      setFont(&Picopixel);
      setCursor(5, keyboardHeight + 31);
      print("shift");
      break;
    case ' ': // space key
      drawLine(39, keyboardHeight + 41, 84, keyboardHeight + 41, ST7735_WHITE);
      break;
    case BACKSPACE:
      drawBackspace(6, keyboardHeight + 18, ST7735_WHITE);
      break;
    case ENTER:
      drawEnter(112, keyboardHeight + 18, ST7735_WHITE);
      break;
    }
    setFont(oldFont);
    setTextColor(oldColour);
  }

  void changeLetter(KeyboardDirection direction)
  {
    deselectKey(currentLetter);
    switch (currentLetter)
    {
    case '1':
      switch (direction)
      {
      case LEFT:
        currentLetter = '-';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '2';
        break;
      case DOWN:
        currentLetter = 'q';
        break;
      }
      break;
    case '2':
      switch (direction)
      {
      case LEFT:
        currentLetter = '1';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '3';
        break;
      case DOWN:
        currentLetter = 'w';
        break;
      }
      break;
    case '3':
      switch (direction)
      {
      case LEFT:
        currentLetter = '2';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '4';
        break;
      case DOWN:
        currentLetter = 'e';
        break;
      }
      break;
    case '4':
      switch (direction)
      {
      case LEFT:
        currentLetter = '3';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '5';
        break;
      case DOWN:
        currentLetter = 'r';
        break;
      }
      break;
    case '5':
      switch (direction)
      {
      case LEFT:
        currentLetter = '4';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '6';
        break;
      case DOWN:
        currentLetter = 'r';
        break;
      }
      break;
    case '6':
      switch (direction)
      {
      case LEFT:
        currentLetter = '5';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '7';
        break;
      case DOWN:
        currentLetter = 't';
        break;
      }
      break;
    case '7':
      switch (direction)
      {
      case LEFT:
        currentLetter = '6';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '8';
        break;
      case DOWN:
        currentLetter = 'u';
        break;
      }
      break;
    case '8':
      switch (direction)
      {
      case LEFT:
        currentLetter = '7';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '9';
        break;
      case DOWN:
        currentLetter = 'i';
        break;
      }
      break;
    case '9':
      switch (direction)
      {
      case LEFT:
        currentLetter = '8';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '0';
        break;
      case DOWN:
        currentLetter = 'o';
        break;
      }
      break;
    case '0':
      switch (direction)
      {
      case LEFT:
        currentLetter = '9';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '-';
        break;
      case DOWN:
        currentLetter = 'p';
        break;
      }
      break;
    case '-':
      switch (direction)
      {
      case LEFT:
        currentLetter = '0';
        break;
      case UP:
        currentLetter = ' ';
        break;
      case RIGHT:
        currentLetter = '1';
        break;
      case DOWN:
        currentLetter = 'p';
        break;
      }
      break;
    case 'q':
      switch (direction)
      {
      case LEFT:
        currentLetter = ENTER;
        break;
      case UP:
        currentLetter = '1';
        break;
      case RIGHT:
        currentLetter = 'w';
        break;
      case DOWN:
        currentLetter = 'a';
        break;
      }
      break;
    case 'w':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'q';
        break;
      case UP:
        currentLetter = '2';
        break;
      case RIGHT:
        currentLetter = 'e';
        break;
      case DOWN:
        currentLetter = 's';
        break;
      }
      break;
    case 'e':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'w';
        break;
      case UP:
        currentLetter = '3';
        break;
      case RIGHT:
        currentLetter = 'r';
        break;
      case DOWN:
        currentLetter = 'd';
        break;
      }
      break;
    case 'r':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'e';
        break;
      case UP:
        currentLetter = '4';
        break;
      case RIGHT:
        currentLetter = 't';
        break;
      case DOWN:
        currentLetter = 'f';
        break;
      }
      break;
    case 't':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'r';
        break;
      case UP:
        currentLetter = '5';
        break;
      case RIGHT:
        currentLetter = 'y';
        break;
      case DOWN:
        currentLetter = 'g';
        break;
      }
      break;
    case 'y':
      switch (direction)
      {
      case LEFT:
        currentLetter = 't';
        break;
      case UP:
        currentLetter = '6';
        break;
      case RIGHT:
        currentLetter = 'u';
        break;
      case DOWN:
        currentLetter = 'h';
        break;
      }
      break;
    case 'u':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'y';
        break;
      case UP:
        currentLetter = '7';
        break;
      case RIGHT:
        currentLetter = 'i';
        break;
      case DOWN:
        currentLetter = 'j';
        break;
      }
      break;
    case 'i':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'u';
        break;
      case UP:
        currentLetter = '8';
        break;
      case RIGHT:
        currentLetter = 'o';
        break;
      case DOWN:
        currentLetter = 'k';
        break;
      }
      break;
    case 'o':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'i';
        break;
      case UP:
        currentLetter = '9';
        break;
      case RIGHT:
        currentLetter = 'p';
        break;
      case DOWN:
        currentLetter = 'l';
        break;
      }
      break;
    case 'p':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'o';
        break;
      case UP:
        currentLetter = '0';
        break;
      case RIGHT:
        currentLetter = ENTER;
        break;
      case DOWN:
        currentLetter = 'l';
        break;
      }
      break;
    case 'a':
      switch (direction)
      {
      case LEFT:
        currentLetter = BACKSPACE;
        break;
      case UP:
        currentLetter = 'q';
        break;
      case RIGHT:
        currentLetter = 's';
        break;
      case DOWN:
        currentLetter = SHIFT;
        break;
      }
      break;
    case 's':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'a';
        break;
      case UP:
        currentLetter = 'w';
        break;
      case RIGHT:
        currentLetter = 'd';
        break;
      case DOWN:
        currentLetter = 'z';
        break;
      }
      break;
    case 'd':
      switch (direction)
      {
      case LEFT:
        currentLetter = 's';
        break;
      case UP:
        currentLetter = 'e';
        break;
      case RIGHT:
        currentLetter = 'f';
        break;
      case DOWN:
        currentLetter = 'x';
        break;
      }
      break;
    case 'f':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'd';
        break;
      case UP:
        currentLetter = 'r';
        break;
      case RIGHT:
        currentLetter = 'g';
        break;
      case DOWN:
        currentLetter = 'c';
        break;
      }
      break;
    case 'g':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'f';
        break;
      case UP:
        currentLetter = 't';
        break;
      case RIGHT:
        currentLetter = 'h';
        break;
      case DOWN:
        currentLetter = 'v';
        break;
      }
      break;
    case 'h':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'g';
        break;
      case UP:
        currentLetter = 'y';
        break;
      case RIGHT:
        currentLetter = 'j';
        break;
      case DOWN:
        currentLetter = 'b';
        break;
      }
      break;
    case 'j':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'h';
        break;
      case UP:
        currentLetter = 'u';
        break;
      case RIGHT:
        currentLetter = 'k';
        break;
      case DOWN:
        currentLetter = 'n';
        break;
      }
      break;
    case 'k':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'j';
        break;
      case UP:
        currentLetter = 'i';
        break;
      case RIGHT:
        currentLetter = 'l';
        break;
      case DOWN:
        currentLetter = 'm';
        break;
      }
      break;
    case 'l':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'k';
        break;
      case UP:
        currentLetter = 'o';
        break;
      case RIGHT:
        currentLetter = ENTER;
        break;
      case DOWN:
        currentLetter = 'm';
        break;
      }
      break;
    case 'z':
      switch (direction)
      {
      case LEFT:
        currentLetter = SHIFT;
        break;
      case UP:
        currentLetter = 's';
        break;
      case RIGHT:
        currentLetter = 'x';
        break;
      case DOWN:
        currentLetter = ' ';
        break;
      }
      break;
    case 'x':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'z';
        break;
      case UP:
        currentLetter = 'd';
        break;
      case RIGHT:
        currentLetter = 'c';
        break;
      case DOWN:
        currentLetter = ' ';
        break;
      }
      break;
    case 'c':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'x';
        break;
      case UP:
        currentLetter = 'f';
        break;
      case RIGHT:
        currentLetter = 'v';
        break;
      case DOWN:
        currentLetter = ' ';
        break;
      }
      break;
    case 'v':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'c';
        break;
      case UP:
        currentLetter = 'g';
        break;
      case RIGHT:
        currentLetter = 'b';
        break;
      case DOWN:
        currentLetter = ' ';
        break;
      }
      break;
    case 'b':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'v';
        break;
      case UP:
        currentLetter = 'h';
        break;
      case RIGHT:
        currentLetter = 'n';
        break;
      case DOWN:
        currentLetter = ' ';
        break;
      }
      break;
    case 'n':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'b';
        break;
      case UP:
        currentLetter = 'j';
        break;
      case RIGHT:
        currentLetter = 'm';
        break;
      case DOWN:
        currentLetter = ' ';
        break;
      }
      break;
    case 'm':
      switch (direction)
      {
      case LEFT:
        currentLetter = 'n';
        break;
      case UP:
        currentLetter = 'k';
        break;
      case RIGHT:
        currentLetter = ENTER;
        break;
      case DOWN:
        currentLetter = ' ';
        break;
      }
      break;
    case SHIFT:
      switch (direction)
      {
      case LEFT:
        currentLetter = 'm';
        break;
      case UP:
        currentLetter = BACKSPACE;
        break;
      case RIGHT:
        currentLetter = 'z';
        break;
      case DOWN:
        currentLetter = ' ';
        break;
      }
      break;
    case ' ':
      switch (direction)
      {
      case LEFT:
        currentLetter = SHIFT;
        break;
      case UP:
        currentLetter = 'c';
        break;
      case RIGHT:
        currentLetter = 'm';
        break;
      case DOWN:
        currentLetter = '6';
        break;
      }
      break;
    case BACKSPACE:
      switch (direction)
      {
      case LEFT:
        currentLetter = ENTER;
        break;
      case UP:
        currentLetter = 'q';
        break;
      case RIGHT:
        currentLetter = 'a';
        break;
      case DOWN:
        currentLetter = SHIFT;
        break;
      }
      break;
    case ENTER:
      switch (direction)
      {
      case LEFT:
        currentLetter = 'l';
        break;
      case UP:
        currentLetter = '-';
        break;
      case RIGHT:
        currentLetter = BACKSPACE;
        break;
      case DOWN:
        currentLetter = ' ';
        break;
      }
      break;
    }
    selectKey(currentLetter);
  }

  char getCurrentLetter()
  {
    if (currentLetter < 0x61)
    {
      return currentLetter;
    }
    else
    {
      if (caps)
        return currentLetter - 0x20;
      else
        return currentLetter;
    }
  }

  String inputKey(String currentWord)
  {
    switch (currentLetter)
    {
    case SHIFT:
      putKeyboard(keyboardHeight, !caps);
      return "";
      break;
    case BACKSPACE:
      currentWord.remove(currentWord.length() - 1);
      return currentWord;
      break;
    case ENTER:
      return currentWord;
      break;
    default:
      return currentWord + getCurrentLetter();
      break;
    }
  }

  void putKeypad(int height)
  {
    GFXfont *oldFont = gfxFont;
    keypadHeight = height;
    fillRect(17, keypadHeight, 95, 42, ST7735_BLACK);
    drawLine(40, keypadHeight + 42, 40, keypadHeight, TRADING21BLUE);
    drawLine(64, keypadHeight + 42, 64, keypadHeight, TRADING21BLUE);
    drawLine(88, keypadHeight + 42, 88, keypadHeight, TRADING21BLUE);
    drawLine(16, keypadHeight, 112, keypadHeight, TRADING21BLUE);
    drawLine(16, keypadHeight + 14, 112, keypadHeight + 14, TRADING21BLUE);
    drawLine(16, keypadHeight + 28, 112, keypadHeight + 28, TRADING21BLUE);
    drawRect(101, keypadHeight - 78, 12, 12, TRADING21BLUE);
    drawCross(104, keypadHeight - 75, ST7735_WHITE);
    setCursor(26, keypadHeight + 4);
    print("1");
    setCursor(50, keypadHeight + 4);
    print("2");
    setCursor(74, keypadHeight + 4);
    print("3");
    setCursor(92, keypadHeight + 4);
    print("DEL");
    setCursor(26, keypadHeight + 18);
    print("4");
    setCursor(50, keypadHeight + 18);
    print("5");
    setCursor(74, keypadHeight + 18);
    print("6");
    setCursor(97, keypadHeight + 18);
    print(".");
    setCursor(26, keypadHeight + 32);
    print("7");
    setCursor(50, keypadHeight + 32);
    print("8");
    setCursor(74, keypadHeight + 32);
    print("9");
    setCursor(98, keypadHeight + 32);
    print("0");
    drawRect(99, keypadHeight - 12, 14, 13, TRADING21BLUE);
    drawTick(101, keypadHeight - 6, ST7735_WHITE);

    setCursor(42, keypadHeight - 44);
    println("Quantity");
    setCursor(42, keypadHeight - 36);
    println("Value");
    setCursor(42, keypadHeight - 28);
    print("Limit");
    drawCircle(35, keypadHeight - 42, 3, ST7735_WHITE);
    drawCircle(35, keypadHeight - 33, 3, ST7735_WHITE);
    drawRect(32, keypadHeight - 27, 7, 6, ST7735_WHITE);
    selectNum(currentNum);
    quantity ? fillCircle(35, keypadHeight - 42, 1, ST7735_WHITE) : fillCircle(35, keypadHeight - 33, 1, ST7735_WHITE);
    limit ? drawTickSmall(33, keypadHeight - 25, ST7735_WHITE) : drawTickSmall(33, keypadHeight - 25, ST7735_BLACK);
    drawRect(32, keypadHeight - 27, 7, 6, ST7735_WHITE);
  }

  void selectNum(char num)
  {
    uint16_t oldColour = textcolor;
    setTextColor(TRADING21BLUE);
    switch (num)
    {
    case '1':
      setCursor(26, keypadHeight + 4);
      print("1");
      break;
    case '2':
      setCursor(50, keypadHeight + 4);
      print("2");
      break;
    case '3':
      setCursor(74, keypadHeight + 4);
      print("3");
      break;
    case '4':
      setCursor(26, keypadHeight + 18);
      print("4");
      break;
    case '5':
      setCursor(50, keypadHeight + 18);
      print("5");
      break;
    case '6':
      setCursor(74, keypadHeight + 18);
      print("6");
      break;
    case '.':
      setCursor(97, keypadHeight + 18);
      print(".");
      break;
    case '7':
      setCursor(26, keypadHeight + 32);
      print("7");
      break;
    case '8':
      setCursor(50, keypadHeight + 32);
      print("8");
      break;
    case '9':
      setCursor(74, keypadHeight + 32);
      print("9");
      break;
    case '0':
      setCursor(98, keypadHeight + 32);
      print("0");
      break;
    case ENTER:
      fillRect(100, keypadHeight - 11, 12, 11, ST7735_GREEN);
      drawTick(101, keypadHeight - 6, ST7735_BLACK);
      break;
    case QUANTITY:
      setCursor(42, keypadHeight - 44);
      println("Quantity");
      break;
    case VALUE:
      setCursor(42, keypadHeight - 36);
      println("Value");
      break;
    case LIMIT:
      setCursor(42, keypadHeight - 28);
      print("Limit");
      break;
    case 'x':
      fillRect(102, keypadHeight - 77, 10, 10, ST7735_RED);
      drawCross(104, keypadHeight - 75, ST7735_WHITE);
      break;
    case BACKSPACE:
      setCursor(92, keypadHeight + 4);
      print("DEL");
      break;
    }
    setTextColor(oldColour);
  }

  void deSelectNum(char num)
  {
    uint16_t oldColour = textcolor;
    setTextColor(ST7735_WHITE);
    switch (num)
    {
    case '1':
      setCursor(26, keypadHeight + 4);
      print("1");
      break;
    case '2':
      setCursor(50, keypadHeight + 4);
      print("2");
      break;
    case '3':
      setCursor(74, keypadHeight + 4);
      print("3");
      break;
    case '4':
      setCursor(26, keypadHeight + 18);
      print("4");
      break;
    case '5':
      setCursor(50, keypadHeight + 18);
      print("5");
      break;
    case '6':
      setCursor(74, keypadHeight + 18);
      print("6");
      break;
    case '.':
      setCursor(97, keypadHeight + 18);
      print(".");
      break;
    case '7':
      setCursor(26, keypadHeight + 32);
      print("7");
      break;
    case '8':
      setCursor(50, keypadHeight + 32);
      print("8");
      break;
    case '9':
      setCursor(74, keypadHeight + 32);
      print("9");
      break;
    case '0':
      setCursor(98, keypadHeight + 32);
      print("0");
      break;
    case ENTER:
      fillRect(100, keypadHeight - 11, 12, 11, ST7735_BLACK);
      drawTick(101, keypadHeight - 6, ST7735_WHITE);
      break;
    case QUANTITY:
      setCursor(42, keypadHeight - 44);
      println("Quantity");
      break;
    case VALUE:
      setCursor(42, keypadHeight - 36);
      println("Value");
      break;
    case LIMIT:
      setCursor(42, keypadHeight - 28);
      print("Limit");
      break;
    case 'x':
      fillRect(102, keypadHeight - 77, 10, 10, ST7735_BLACK);
      drawCross(104, keypadHeight - 75, ST7735_WHITE);
      break;
    case BACKSPACE:
      setCursor(92, keypadHeight + 4);
      print("DEL");
      break;
    }
    setTextColor(oldColour);
  }

  char getCurrentNum()
  {
    return currentNum;
  }

  void changeNum(KeyboardDirection direction)
  {
    deSelectNum(currentNum);
    switch (currentNum)
    {
    case '1':
      switch (direction)
      {
      case LEFT:
        currentNum = BACKSPACE;
        break;
      case UP:
        currentNum = LIMIT;
        break;
      case RIGHT:
        currentNum = '2';
        break;
      case DOWN:
        currentNum = '4';
        break;
      }
      break;
    case '2':
      switch (direction)
      {
      case LEFT:
        currentNum = '1';
        break;
      case UP:
        currentNum = LIMIT;
        break;
      case RIGHT:
        currentNum = '3';
        break;
      case DOWN:
        currentNum = '5';
        break;
      }
      break;
    case '3':
      switch (direction)
      {
      case LEFT:
        currentNum = '2';
        break;
      case UP:
        currentNum = LIMIT;
        break;
      case RIGHT:
        currentNum = BACKSPACE;
        break;
      case DOWN:
        currentNum = '6';
        break;
      }
      break;
    case ENTER:
      switch (direction)
      {
      case LEFT:
        currentNum = LIMIT;
        break;
      case UP:
        currentNum = 'x';
        break;
      case DOWN:
        currentNum = BACKSPACE;
        break;
      }
      break;
    case '4':
      switch (direction)
      {
      case LEFT:
        currentNum = '.';
        break;
      case UP:
        currentNum = '1';
        break;
      case RIGHT:
        currentNum = '5';
        break;
      case DOWN:
        currentNum = '7';
        break;
      }
      break;
    case '5':
      switch (direction)
      {
      case LEFT:
        currentNum = '4';
        break;
      case UP:
        currentNum = '2';
        break;
      case RIGHT:
        currentNum = '6';
        break;
      case DOWN:
        currentNum = '8';
        break;
      }
      break;
    case '6':
      switch (direction)
      {
      case LEFT:
        currentNum = '5';
        break;
      case UP:
        currentNum = '3';
        break;
      case RIGHT:
        currentNum = '.';
        break;
      case DOWN:
        currentNum = '9';
        break;
      }
      break;
    case '.':
      switch (direction)
      {
      case LEFT:
        currentNum = '6';
        break;
      case UP:
        currentNum = BACKSPACE;
        break;
      case RIGHT:
        currentNum = '4';
        break;
      case DOWN:
        currentNum = '0';
        break;
      }
      break;
    case '7':
      switch (direction)
      {
      case LEFT:
        currentNum = '0';
        break;
      case UP:
        currentNum = '4';
        break;
      case RIGHT:
        currentNum = '8';
        break;
      case DOWN:
        currentNum = 'x';
        break;
      }
      break;
    case '8':
      switch (direction)
      {
      case LEFT:
        currentNum = '7';
        break;
      case UP:
        currentNum = '5';
        break;
      case RIGHT:
        currentNum = '9';
        break;
      case DOWN:
        currentNum = 'x';
        break;
      }
      break;
    case '9':
      switch (direction)
      {
      case LEFT:
        currentNum = '8';
        break;
      case UP:
        currentNum = '6';
        break;
      case RIGHT:
        currentNum = '0';
        break;
      case DOWN:
        currentNum = 'x';
        break;
      }
      break;
    case '0':
      switch (direction)
      {
      case LEFT:
        currentNum = '9';
        break;
      case UP:
        currentNum = '.';
        break;
      case RIGHT:
        currentNum = '7';
        break;
      case DOWN:
        currentNum = 'x';
        break;
      }
      break;
    case 'x':
      switch (direction)
      {
      case LEFT:
        currentNum = QUANTITY;
        break;
      case UP:
        currentNum = '0';
        break;
      case DOWN:
        currentNum = ENTER;
        break;
      }
      break;
    case BACKSPACE:
      switch (direction)
      {
      case LEFT:
        currentNum = '3';
        break;
      case UP:
        currentNum = ENTER;
        break;
      case RIGHT:
        currentNum = '1';
      case DOWN:
        currentNum = '.';
        break;
      }
      break;
    case QUANTITY:
      switch (direction)
      {
      case UP:
        currentNum = 'x';
        break;
      case RIGHT:
        currentNum = 'x';
        break;
      case DOWN:
        currentNum = VALUE;
        break;
      }
      break;
    case VALUE:
      switch (direction)
      {
      case UP:
        currentNum = QUANTITY;
        break;
      case DOWN:
        currentNum = LIMIT;
        break;
      }
      break;
    case LIMIT:
      switch (direction)
      {
      case UP:
        currentNum = VALUE;
        break;
      case RIGHT:
        currentNum = ENTER;
        break;
      case DOWN:
        currentNum = '1';
        break;
      }
      break;
    }
    selectNum(currentNum);
  }

  String inputNum(String currentDigits)
  {
    switch (currentNum)
    {
    case QUANTITY:
      quantity = true;
      return "";
      break;
    case VALUE:
      quantity = false;
      return "";
      break;
    case LIMIT:
      limit = !limit;
      return "";
      break;
    case BACKSPACE:
      currentDigits.remove(currentDigits.length() - 1);
      return currentDigits;
      break;
    case ENTER:
      return currentDigits;
    case 'x':
      return "EXIT";
    default:
      
      if (getCurrentNum() == '.' and currentDigits.indexOf('.') != -1)
        return currentDigits;
      else if (currentDigits == "0" and getCurrentNum() == '0')
        return currentDigits;
      else if (currentDigits.substring(currentDigits.indexOf('.')).length() >= 3)
        return currentDigits;
      else
        return currentDigits + getCurrentNum();
    }
  }

  String takeNumInput(int x, int y)
  {
    String in;
    String tftReturn;
    String toReturn;
    bool done = false;
    drawRect(x, y, 69, 12, TRADING21BLUE);
    while (!done)
    {
      while (Serial.available())
      {
        in = Serial.readString();
        if (in == "a")
        {
          changeNum(LEFT);
        }
        else if (in == "w")
        {
          changeNum(UP);
        }
        else if (in == "d")
        {
          changeNum(RIGHT);
        }
        else if (in == "s")
        {
          changeNum(DOWN);
        }
        else if (in == "l")
        {
          tftReturn = inputNum(toReturn);
          if (!tftReturn.isEmpty() or getCurrentNum() == BACKSPACE)
          {
            toReturn = tftReturn;
          }
          setCursor(x + 3, y + 3);
          fillRect(x + 1, y + 1, 67, 10, ST7735_BLACK);
          if (toReturn != "EXIT")
            if (quantity)
              print(toReturn);
            else
              printMoney(GBP, toReturn.toFloat());
          switch (getCurrentNum())
          {
          case QUANTITY:
            fillCircle(35, keypadHeight - 42, 1, ST7735_BLACK);
            fillCircle(35, keypadHeight - 33, 1, ST7735_BLACK);
            if (quantity)
              fillCircle(35, keypadHeight - 42, 1, ST7735_WHITE);
            else
              fillCircle(35, keypadHeight - 33, 1, ST7735_WHITE);
            break;
          case VALUE:
            fillCircle(35, keypadHeight - 42, 1, ST7735_BLACK);
            fillCircle(35, keypadHeight - 33, 1, ST7735_BLACK);
            if (!quantity)
              fillCircle(35, keypadHeight - 33, 1, ST7735_WHITE);
            else
              fillCircle(35, keypadHeight - 42, 1, ST7735_WHITE);
            break;
          case LIMIT:
            if (limit)
              drawTickSmall(33, keypadHeight - 25, ST7735_WHITE);
            else
              drawTickSmall(33, keypadHeight - 25, ST7735_BLACK);
            drawRect(32, keypadHeight - 27, 7, 6, ST7735_WHITE);
            break;
          case 'x':
            return "";
          case ENTER:
            return toReturn;
          }
          Serial.println("quantity: " + String(quantity));
          Serial.println("limit " + String(limit));
        }
      }
    }
    return toReturn;
  }

private:
  int keyboardHeight;
  int keypadHeight;
  bool caps;
  bool quantity = true;
  bool limit = false;
  char currentLetter = '1';
  char currentNum = QUANTITY;
};