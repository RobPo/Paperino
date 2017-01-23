//*********************************************************************
//PAPERINO: Hello World example. The shortest possible example code ;-)
//*********************************************************************

#include "Adafruit_mfGFX.h"
#include "fonts.h"
#include "PL_microEPD.h"

#define EPD_RST     A0
#define EPD_BUSY    A1
#define EPD_CS      D5

PL_microEPD display(EPD_CS, EPD_RST, EPD_BUSY);  

void setup() {
	SPI1.begin();
    SPI1.setBitOrder(MSBFIRST);                 
    SPI1.setDataMode(SPI_MODE0); 
    SPI1.setClockDivider(SPI_CLOCK_DIV4);
    display.begin(true);                    //Initializing. TRUE=Initial update to refresh the screen to defined state
    display.print("Hello World!");          //Writes content into image buffer
    display.updateFull();                   //Triggers a full image update, showing content of the imagebuffer
}

void loop() {
}

