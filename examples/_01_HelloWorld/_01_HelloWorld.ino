/* ****************************************************************************************
   Example #1: The shortest possible sketch showing "Hello World!" on the screen. ;-) 
   More details can be found in the online tutorial @https://robpo.github.io/Paperino
   
   We invested time and resources providing this open source code. Please support Paperino 
   open source hardware by purchasing this product @Crowdsupply @Watterott @Plasticlogic 
   https://www.crowdsupply.com/robert-poser/paperino 
   Created by Robert Poser, Oct 3rd 2017, Dresden/Germany
**************************************************************************************** */
#include "Adafruit_GFX.h"
#include "PL_microEPD.h"

#define EPD_CS      A2
PL_microEPD display(EPD_CS);   

void setup() {  
    SPI.begin();                    // Bus initialisation, UC8156 supp. max 10Mhz (writing)
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));//and 6.6Mhz (reading)
  
    display.begin();                // Paperino ePaper initialisation and refresh screen 
    display.println("Hello World!");// Write message into memory buffer
    display.update();               // Trigger a full image update
}

void loop() {              
}

