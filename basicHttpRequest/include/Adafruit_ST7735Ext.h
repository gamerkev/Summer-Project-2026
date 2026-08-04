#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <keyboardFont.h>
#include <Fonts/Picopixel.h>

#define TRADING21BLUE 0x053b

#define TFT_CS 12   // Pins for the SPI interface
#define TFT_RST 3
#define TFT_DC 2
#define TFT_MOSI 11
#define TFT_SCLK 7

class Adafruit_ST7735Ext : public Adafruit_ST7735{    // Extend the display library to be able to cleanly add functionality
  public:
    Adafruit_ST7735Ext (int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : Adafruit_ST7735(cs, dc, mosi, sclk, rst){}  // Same constructor as the class it's extending

    void movingText(String toWrite, int y){
      // int pixelLength = toWrite.length() * 6;                       // 6 pixels width each character
      int extraChars = toWrite.length() - 18;
      // int extraPixels = (ceil((pixelLength - 114)/6) * 6);    // chose the numbers from this line through trial and error
      fillRect(1, y, 126, 8, ST7735_BLACK);                  // black out the line where the text will be
      setCursor(10, y);
      print(toWrite);
      fillRect(1, y, 9, 8, ST7735_BLACK);                   // black out the edges of the text
      fillRect(118, y, 9, 8, ST7735_BLACK);
      drawRect(0, 0, 128, 160, TRADING21BLUE);
      if(extraChars>0){
        delay(1000);                                                  // time to read the first part of the text
      } else{
        delay(500);
      }
      //THE MAX LENGTH OF TEXT THAT IS PADDED BY 9 PIXELS EITHER SIDE IS 18 CHARACTERS
      for(int i = 0; i < (extraChars*6)+1; i=i+2){
        fillRect(1, y, 128, 8, ST7735_BLACK);
        setCursor(10-i, y);                                     // move 1 pixel to the left
        print(toWrite);
        fillRect(1, y, 9, 8, ST7735_BLACK);
        fillRect(118, y, 9, 8, ST7735_BLACK);
        drawRect(0, 0, 128, 160, TRADING21BLUE);
        delay(50);
      }
    }

    void logo(){
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

    void putKeyboard(int height, bool capital){
      GFXfont* oldFont = gfxFont;
      keyBoardHeight = height;
      setFont(&Keyboard);
      fillRect(1, keyBoardHeight-13, 126, 61, ST7735_BLACK);
      //Top row
      setCursor(11, keyBoardHeight);
      print("1");
      setCursor(21, keyBoardHeight);
      print("2");
      setCursor(31, keyBoardHeight);
      print("3");
      setCursor(41, keyBoardHeight);
      print("4");
      setCursor(51, keyBoardHeight);
      print("5");
      setCursor(61, keyBoardHeight);
      print("6");
      setCursor(71, keyBoardHeight);
      print("7");
      setCursor(81, keyBoardHeight);
      print("8");
      setCursor(91, keyBoardHeight);
      print("9");
      setCursor(101, keyBoardHeight);
      print("0");
      setCursor(111, keyBoardHeight);
      print("-");
      if(!capital){
        //Second row
        setCursor(16, keyBoardHeight+12);
        print("q");
        setCursor(26, keyBoardHeight+12);
        print("w");
        setCursor(36, keyBoardHeight+12);
        print("e");
        setCursor(46, keyBoardHeight+12);
        print("r");
        setCursor(56, keyBoardHeight+12);
        print("t");
        setCursor(66, keyBoardHeight+12);
        print("y");
        setCursor(76, keyBoardHeight+12);
        print("u");
        setCursor(86, keyBoardHeight+12);
        print("i");
        setCursor(96, keyBoardHeight+12);
        print("o");
        setCursor(106, keyBoardHeight+12);
        print("p");
        //Third row
        setCursor(21, keyBoardHeight+24);
        print("a");
        setCursor(31, keyBoardHeight+24);
        print("s");
        setCursor(41, keyBoardHeight+24);
        print("d");
        setCursor(51, keyBoardHeight+24);
        print("f");
        setCursor(61, keyBoardHeight+24);
        print("g");
        setCursor(71, keyBoardHeight+24);
        print("h");
        setCursor(81, keyBoardHeight+24);
        print("j");
        setCursor(91, keyBoardHeight+24);
        print("k");
        setCursor(101, keyBoardHeight+24);
        print("l");
        //final row
        setCursor(31, keyBoardHeight+36);
        print("z");
        setCursor(41, keyBoardHeight+36);
        print("x");
        setCursor(51, keyBoardHeight+36);
        print("c");
        setCursor(61, keyBoardHeight+36);
        print("v");
        setCursor(71, keyBoardHeight+36);
        print("b");
        setCursor(81, keyBoardHeight+36);
        print("n");
        setCursor(91, keyBoardHeight+36);
        print("m");
      } else{
        //Second row
        setCursor(16, keyBoardHeight+12);
        print("Q");
        setCursor(26, keyBoardHeight+12);
        print("W");
        setCursor(36, keyBoardHeight+12);
        print("E");
        setCursor(46, keyBoardHeight+12);
        print("R");
        setCursor(56, keyBoardHeight+12);
        print("T");
        setCursor(66, keyBoardHeight+12);
        print("Y");
        setCursor(76, keyBoardHeight+12);
        print("U");
        setCursor(86, keyBoardHeight+12);
        print("I");
        setCursor(96, keyBoardHeight+12);
        print("O");
        setCursor(106, keyBoardHeight+12);
        print("P");
        //Third row
        setCursor(21, keyBoardHeight+24);
        print("A");
        setCursor(31, keyBoardHeight+24);
        print("S");
        setCursor(41, keyBoardHeight+24);
        print("D");
        setCursor(51, keyBoardHeight+24);
        print("F");
        setCursor(61, keyBoardHeight+24);
        print("G");
        setCursor(71, keyBoardHeight+24);
        print("H");
        setCursor(81, keyBoardHeight+24);
        print("J");
        setCursor(91, keyBoardHeight+24);
        print("K");
        setCursor(101, keyBoardHeight+24);
        print("L");
        //final row
        setCursor(31, keyBoardHeight+36);
        print("Z");
        setCursor(41, keyBoardHeight+36);
        print("X");
        setCursor(51, keyBoardHeight+36);
        print("C");
        setCursor(61, keyBoardHeight+36);
        print("V");
        setCursor(71, keyBoardHeight+36);
        print("B");
        setCursor(81, keyBoardHeight+36);
        print("N");
        setCursor(91, keyBoardHeight+36);
        print("M");
      }
      //shift key
      setFont(&Picopixel);
      setCursor(5, keyBoardHeight+31);
      print("shift");
      //spacebar
      drawLine(39, keyBoardHeight+41, 84, keyBoardHeight+41, ST7735_WHITE);
      setFont(oldFont);
    }

    void selectKey(char key){
      switch(key){
        case '1':
          drawEllipse(12, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '2':
          drawEllipse(23, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '3':
          drawEllipse(33, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '4':
          drawEllipse(43, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '5':
          drawEllipse(53, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '6':
          drawEllipse(63, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '7':
          drawEllipse(73, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '8':
          drawEllipse(83, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '9':
          drawEllipse(93, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '0':
          drawEllipse(103, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case '-':
          drawEllipse(113, keyBoardHeight-7, 5, 6, TRADING21BLUE);
          break;
        case 'q':
          drawEllipse(18, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 'w':
          drawEllipse(28, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 'e':
          drawEllipse(38, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 'r':
          drawEllipse(48, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 't':
          drawEllipse(58, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 'y':
          drawEllipse(68, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 'u':
          drawEllipse(78, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 'i':
          drawEllipse(88, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 'o':
          drawEllipse(98, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 'p':
          drawEllipse(108, keyBoardHeight+5, 5, 8, TRADING21BLUE);
          break;
        case 'a':
          drawEllipse(23, keyBoardHeight+17, 5, 8, TRADING21BLUE);
          break;
        case 's':
          drawEllipse(33, keyBoardHeight+17, 5, 8, TRADING21BLUE);
          break;
        case 'd':
          drawEllipse(43, keyBoardHeight+17, 5, 8, TRADING21BLUE);
          break;
        case 'f':
          drawEllipse(53, keyBoardHeight+17, 5, 8, TRADING21BLUE);
          break;
        case 'g':
          drawEllipse(63, keyBoardHeight+17, 5, 8, TRADING21BLUE);
          break;
        case 'h':
          drawEllipse(73, keyBoardHeight+17, 5, 8, TRADING21BLUE);
          break;
        case 'j':
          drawEllipse(83, keyBoardHeight+17, 5, 8, TRADING21BLUE);
          break;
        case 'k':
          drawEllipse(93, keyBoardHeight+17, 5, 8, TRADING21BLUE);
          break;
        case 'l':
          drawEllipse(103, keyBoardHeight+17, 5, 8, TRADING21BLUE);
          break;
        case 'z':
          drawEllipse(33, keyBoardHeight+29, 5, 8, TRADING21BLUE);
          break;
        case 'x':
          drawEllipse(43, keyBoardHeight+29, 5, 8, TRADING21BLUE);
          break;
        case 'c':
          drawEllipse(53, keyBoardHeight+29, 5, 8, TRADING21BLUE);
          break;
        case 'v':
          drawEllipse(63, keyBoardHeight+29, 5, 8, TRADING21BLUE);
          break;
        case 'b':
          drawEllipse(73, keyBoardHeight+29, 5, 8, TRADING21BLUE);
          break;
        case 'n':
          drawEllipse(83, keyBoardHeight+29, 5, 8, TRADING21BLUE);
          break;
        case 'm':
          drawEllipse(93, keyBoardHeight+29, 5, 8, TRADING21BLUE);
          break;
        case '/': //shift key
          drawEllipse(12, keyBoardHeight+29, 9, 6, TRADING21BLUE);
          break;
        case ' ': //space key
          drawEllipse(61, keyBoardHeight+41, 22, 4, TRADING21BLUE);
          break;
      }
    }

    private:
      int keyBoardHeight;
};