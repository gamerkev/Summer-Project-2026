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

    void putKeyboard(){
      //Top row
      setCursor(9, 100);
      print("1");
      setCursor(19, 100);
      print("2");
      setCursor(29, 100);
      print("3");
      setCursor(39, 100);
      print("4");
      setCursor(49, 100);
      print("5");
      setCursor(59, 100);
      print("6");
      setCursor(69, 100);
      print("7");
      setCursor(79, 100);
      print("8");
      setCursor(89, 100);
      print("9");
      setCursor(99, 100);
      print("0");
      setCursor(109, 100);
      print("-");
      //Second row
      setCursor(14, 112);
      print("q");
      setCursor(24, 112);
      print("w");
      setCursor(34, 112);
      print("e");
      setCursor(44, 112);
      print("r");
      setCursor(54, 112);
      print("t");
      setCursor(64, 112);
      print("y");
      setCursor(74, 112);
      print("u");
      setCursor(84, 112);
      print("i");
      setCursor(94, 112);
      print("o");
      setCursor(104, 112);
      print("p");
      //Third row
      setCursor(19, 124);
      print("a");
      setCursor(29, 124);
      print("s");
      setCursor(39, 124);
      print("d");
      setCursor(49, 124);
      print("f");
      setCursor(59, 124);
      print("g");
      setCursor(69, 124);
      print("h");
      setCursor(79, 124);
      print("j");
      setCursor(89, 124);
      print("k");
      setCursor(99, 124);
      print("l");
      //final row
      setCursor(29, 136);
      print("z");
      setCursor(39, 136);
      print("x");
      setCursor(49, 136);
      print("c");
      setCursor(59, 136);
      print("v");
      setCursor(69, 136);
      print("b");
      setCursor(79, 136);
      print("n");
      setCursor(89, 136);
      print("m");
      //spacebar
      drawLine(39, 143, 84, 143, ST7735_WHITE);
    }
    void putCapitalKeyboard(){

    }
};