/* ****************************************************************************************
   Example #6: A finger-tap raises an interrupt on INT1 which wakes-up the microcontroller
   assuming it is connected with the wake-up pin (WKP). After updating the screen the MCU 
   goes back to deep sleep to reduce its power consumption.
   More details can be found in the online tutorial @https://robpo.github.io/Paperino
   
   We invested time and resources providing this open source code. Please support Paperino 
   open source hardware by purchasing this product @Crowdsupply @Paperino @Plasticlogic 
   https://www.crowdsupply.com/robert-poser/paperino 
   Created by Robert Poser, May 18th 2017, Dresden/Germany
**************************************************************************************** */
SYSTEM_MODE(SEMI_AUTOMATIC);
STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

#define EPD_RST     A0
#define EPD_BUSY    A1
#define EPD_CS      A2
#define ACC_CS      D6

#include "Adafruit_GFX.h"
#include "PL_microEPD.h"

PL_microEPD display(EPD_CS, EPD_RST, EPD_BUSY);  
BO_BMA250   accel(ACC_CS);
retained 	int i = 0;

void setup() {
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);                 
    SPI.setDataMode(SPI_MODE0); 
    SPI.setClockDivider(SPI_CLOCK_DIV4);
  
    i+=1;
    display.begin(false);
    display.setTextSize(6);
    display.setCursor(0,4);
    display.print(i);
    display.drawCircle(170, 36, 40, EPD_LGRAY);
    display.drawCircle(170, 36, 45, EPD_LGRAY);
    display.drawCircle(170, 36, 50, EPD_LGRAY);
    display.drawCircle(170, 36, 55, EPD_LGRAY);
    display.drawCircle(170, 36, 70, EPD_LGRAY);
    display.drawCircle(170, 36, 75, EPD_LGRAY);
    display.drawCircle(0, 36, 135, EPD_LGRAY);
    display.drawCircle(0, 36, 120, EPD_LGRAY);
    display.drawCircle(0, 36, 95, EPD_LGRAY);
    display.setTextSize(1);
    display.setCursor(0, 52);
    display.print("Please dbl-tap to wakeup& increase the counter!");
    display.update();
    
    accel.begin();
    accel.activateTapOnInt1();
}

void loop() {
    System.sleep(SLEEP_MODE_DEEP, 3600);
}