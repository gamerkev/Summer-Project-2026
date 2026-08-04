#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>

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

    void putKeyboard(int height){
      //Top row
      setCursor(11, height);
      print("1");
      setCursor(21, height);
      print("2");
      setCursor(31, height);
      print("3");
      setCursor(41, height);
      print("4");
      setCursor(51, height);
      print("5");
      setCursor(61, height);
      print("6");
      setCursor(71, height);
      print("7");
      setCursor(81, height);
      print("8");
      setCursor(91, height);
      print("9");
      setCursor(101, height);
      print("0");
      setCursor(111, height);
      print("-");
      //Second row
      setCursor(16, height+12);
      print("q");
      setCursor(26, height+12);
      print("w");
      setCursor(36, height+12);
      print("e");
      setCursor(46, height+12);
      print("r");
      setCursor(56, height+12);
      print("t");
      setCursor(66, height+12);
      print("y");
      setCursor(76, height+12);
      print("u");
      setCursor(86, height+12);
      print("i");
      setCursor(96, height+12);
      print("o");
      setCursor(106, height+12);
      print("p");
      //Third row
      setCursor(21, height+24);
      print("a");
      setCursor(31, height+24);
      print("s");
      setCursor(41, height+24);
      print("d");
      setCursor(51, height+24);
      print("f");
      setCursor(61, height+24);
      print("g");
      setCursor(71, height+24);
      print("h");
      setCursor(81, height+24);
      print("j");
      setCursor(91, height+24);
      print("k");
      setCursor(101, height+24);
      print("l");
      //final row
      setCursor(31, height+36);
      print("z");
      setCursor(41, height+36);
      print("x");
      setCursor(51, height+36);
      print("c");
      setCursor(61, height+36);
      print("v");
      setCursor(71, height+36);
      print("b");
      setCursor(81, height+36);
      print("n");
      setCursor(91, height+36);
      print("m");
      //spacebar
      drawLine(39, height+41, 84, height+41, ST7735_WHITE);
    }
    void putCapitalKeyboard(){

    }
};