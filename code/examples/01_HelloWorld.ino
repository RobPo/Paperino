//PAPERINO: Hello World example for the Paperino Micro ePaper breakoutboard

#include "Adafruit_mfGFX.h"
#include "PL_microEPD.h"
#include "fonts.h"

#define EPD_CS      D5                  //SPI CS of the ePaper screen
#define EPD_RST     A0                  //Reset pin
#define EPD_BUSY    A1                  //Busy pin

PL_microEPD display(EPD_CS, EPD_RST, EPD_BUSY);

void setup() {
    SPI1.begin();
    SPI1.setBitOrder(MSBFIRST);                 
    SPI1.setDataMode(SPI_MODE0); 
    SPI1.setClockDivider(SPI_CLOCK_DIV4);
    display.begin(true);                //Initialize the ePaper and reset the screen
    display.print(“Hello World!”);      //Write “Hello World!” into the buffer
    display.updateFull();               //Trigger an update based on the buffer content
}

void loop() {
}