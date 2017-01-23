//***********************************************************************
// PAPERINO: GFX demo. Showing examples of different fonts and text sizes
// as well as graphic elements like rectangles, circles...
//***********************************************************************
#include "Adafruit_mfGFX.h"
#include "fonts.h"
#include "PL_microEPD.h"

#define EPD_RST     A0
#define EPD_BUSY    A1
#define EPD_CS      D5

PL_microEPD display(EPD_CS, EPD_RST, EPD_BUSY); 
int i=0;

void setup() {
	SPI1.begin();
    SPI1.setBitOrder(MSBFIRST);                 
    SPI1.setDataMode(SPI_MODE0); 
    SPI1.setClockDivider(SPI_CLOCK_DIV4);
    display.begin(true);                    //Initializing. TRUE=Initial update to refresh the screen to defined state
    display.print("Hello World!");          //Writes content into image buffer
    display.updateFull();                   //Triggers a full image update, showing content of the imagebuffer

    delay(5000);
    display.clear();
    for (int y=0; y < 6; y++) {
        for (int x=0; x < 26; x++) {
            i=i+1;
            display.drawChar(5+x*7,4+y*11,i,0,3,1);
        }   
    }
    display.updateFull();   
    
    delay(5000);
    display.clear();
    display.setCursor(1,1);
    display.println("Size1");
    display.setTextSize(2);
    display.println("Size2");
    display.setTextSize(3);
    display.println("Size3");
    display.setCursor(100,5);
    display.setTextSize(9);
    display.println("9");
    display.updateFull();
    
    delay(5000);
    display.invertDisplay();
    display.updateFull();
    
    delay(5000);
    display.clear();
    display.setCursor(1,1);
    display.setTextSize(1);
    display.setFont(TIMESNR_8);
    display.println("Times New Roman");
    display.setFont(CENTURY_8);
    display.println("Century Gothic");
    display.setFont(ARIAL_8);
    display.println("Arial");
    display.setFont(COMICS_8);
    display.println("Comic");
    display.setFont(GLCDFONT);
    display.println("GLCDFONT default");
    display.setFont(TEST);
    display.println("Test");
    display.println("This is a long text with automatic line wrapping.");
    display.updateFull();
    
    delay(5000);
    display.clear();
    display.drawRect(5,5,50,50,0);
    display.fillRect(40,40,25,25,0);
    display.drawCircle(100,35,30,0);
    display.drawCircle(100,35,15,0);
    display.drawCircle(100,35,8,0);
    display.drawCircle(100,35,4,0);
    display.updateFull();
    
    delay(5000);
    display.clear();
    display.setTextSize(2);
    display.setCursor(5,20);
    display.println("Hello");
    display.setCursor(5,36);
    display.println("World!");
    display.drawCircle(170,36,40,0);
    display.drawCircle(170,36,45,0);
    display.drawCircle(170,36,50,0);
    display.drawCircle(170,36,55,0);
    display.drawCircle(170,36,70,0);
    display.drawCircle(170,36,75,0);
    display.drawCircle(0,36,135,0);
    display.drawCircle(0,36,120,0);
    display.drawCircle(0,36,75,0);
    display.invertDisplay();
    display.updateFull();
}

void loop() {
}

