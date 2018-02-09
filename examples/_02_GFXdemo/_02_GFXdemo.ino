/* ****************************************************************************************
   Example #2: Here we see some more Graphic/GFX basics, like changing the font size, 
   inverting the screen and drawing some circles and rectangles. 
   More details can be found in the online tutorial @https://robpo.github.io/Paperino
   
   We invested time and resources providing this open source code. Please support Paperino 
   open source hardware by purchasing this product @Crowdsupply @Watterott @Plasticlogic 
   https://www.crowdsupply.com/robert-poser/paperino 
   Created by Robert Poser, Oct 3rd 2017, Dresden/Germany
**************************************************************************************** */
#include "Adafruit_GFX.h"
#include "PL_microEPD.h"

#define EPD_RST     A0
#define EPD_BUSY    A1
#define EPD_CS      A2

PL_microEPD display(EPD_CS, EPD_RST, EPD_BUSY);  
int i=0;

void setup() {
    SPI.begin();                            // Bus initialisation, UC8156 supp. max 10Mhz (writing)
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));//and 6.6Mhz (reading)


    display.begin();                        // Paperino ePaper initialisation and refresh screen 
    for (int y=0; y < 6; y++) {
        for (int x=0; x < 26; x++) {
            i=i+1;                          // Draw some ASCII characters
            display.drawChar(1+x*7,1+y*11,i,0,3,1);
        }   
    }
    display.update();                       // Trigger a full image update
    delay(5000);
    
    display.clear();                        // Clear the image buffer
    display.setCursor(1,1);                 // Set Cursor start position 
    display.println("Size1");
    display.setTextSize(2);                 // Change text size
    display.println("Size2");
    display.setTextSize(3);
    display.println("Size3");
    display.setCursor(100,5);
    display.setTextSize(9);
    display.println("9");
    display.update();                       
    delay(5000);

    display.invert();                       // Invert the display content
    display.update();
    delay(5000);

    display.clear();
    display.drawRect(5, 5, 50, 50, EPD_BLACK);          // Draw a rectangle
    display.fillRect(40,40,25,25, EPD_BLACK);        // Draw a filled rectangle 
    display.drawCircle(100, 35, 30, EPD_BLACK);        // Draw a circle
    display.drawCircle(100, 35, 15, EPD_BLACK);
    display.drawCircle(100, 35, 8, EPD_BLACK);
    display.drawCircle(100, 35, 4, EPD_BLACK);
    display.update();
    delay(5000);

    display.clear();
    display.setTextSize(2);
    display.setCursor(5, 20);
    display.println("Hello");
    display.setCursor(5, 36);
    display.println("World!");
    display.drawCircle(170, 36, 40, EPD_BLACK);
    display.drawCircle(170, 36, 45, EPD_BLACK);
    display.drawCircle(170, 36, 50, EPD_BLACK);
    display.drawCircle(170, 36, 55, EPD_BLACK);
    display.drawCircle(170, 36, 70, EPD_BLACK);
    display.drawCircle(170, 36, 75, EPD_BLACK);
    display.drawCircle(0, 36, 135, EPD_BLACK);
    display.drawCircle(0, 36, 120, EPD_BLACK);
    display.drawCircle(0, 36, 75, EPD_BLACK);
    display.invert();
    display.update();
}

void loop() {
}
