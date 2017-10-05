/* ****************************************************************************************
   Example #6: A finger-tap raises an interrupt on INT1 which wakes-up the microcontroller
   assuming it is connected with the wake-up pin (WKP). After updating the screen the MCU 
   goes back to deep sleep to reduce its power consumption.
   More details can be found in the online tutorial @https://robpo.github.io/Paperino
   
   We invested time and resources providing this open source code. Please support Paperino 
   open source hardware by purchasing this product @Crowdsupply @Watterott @Plasticlogic 
   https://www.crowdsupply.com/robert-poser/paperino 
   Created by Robert Poser, Oct 3rd 2017, Dresden/Germany
**************************************************************************************** */
SYSTEM_MODE(SEMI_AUTOMATIC);
STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

#define EPD_CS      A2
#define ACC_CS      D6

#include "Adafruit_GFX.h"
#include "PL_microEPD.h"
 
PL_microEPD display(EPD_CS);  
BO_BMA250   accel(ACC_CS);
retained    int i = 0;

void setup() {
    SPI.begin();                    // Bus initialisation, UC8156 supp. max 10Mhz (writing)
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));//and 6.6Mhz (reading)
  
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
    display.print("Please tap to wakeup &  increase the counter!");
    display.update();
    
    accel.begin();
    accel.activateTapOnInt1();
    accel.clearLatchedInt1();
}

void loop() {
    System.sleep(SLEEP_MODE_DEEP, 3600);
}
