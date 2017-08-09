/* ****************************************************************************************
   Example #1: The shortest possible sketch showing "Hello World!" on the screen. ;-) 
   More details can be found in the online tutorial @https://robpo.github.io/Paperino
   
   We invested time and resources providing this open source code. Please support Paperino 
   open source hardware by purchasing this product @Crowdsupply @Paperino @Plasticlogic 
   https://www.crowdsupply.com/robert-poser/paperino 
   Created by Robert Poser, May 18th 2017, Dresden/Germany
**************************************************************************************** */
#include "Adafruit_GFX.h"
#include "PL_microEPD.h"

#define EPD_RST     A0
#define EPD_BUSY    A1
#define EPD_CS      A2

PL_microEPD display(EPD_CS, EPD_RST, EPD_BUSY);  

void setup() {  
    SPI.begin();                    // SPI-Bus initialisation
    SPI.setBitOrder(MSBFIRST);                 
    SPI.setDataMode(SPI_MODE0); 
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    
    display.begin();                // Paperino ePaper initialisation and refresh screen 
    display.println("Hello World!");// Write message into memory buffer
    display.update();               // Trigger a full image update
}

void loop() {              
}