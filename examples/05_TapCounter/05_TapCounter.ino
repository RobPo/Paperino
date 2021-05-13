/* ****************************************************************************************
   Example #5: A finger-tap raises an interrupt on INT1 which is read-back, detected and
   increasing a counter. 
   More details can be found in the online tutorial @https://robpo.github.io/Paperino
   
   We invested time and resources providing this open source code. Please support Paperino 
   open source hardware by purchasing this product @Crowdsupply @Watterott @Plasticlogic 
   https://www.crowdsupply.com/robert-poser/paperino 
   Created by Robert Poser, Oct 3rd 2017, Dresden/Germany
**************************************************************************************** */
SYSTEM_MODE(SEMI_AUTOMATIC);
#include "Adafruit_GFX.h"
#include "PL_microEPD.h"

#define EPD_RST     A0
#define EPD_BUSY    A1
#define EPD_CS      A2
#define ACC_CS      D6

PL_microEPD display(EPD_CS, EPD_RST, EPD_BUSY);  
BO_BMA250   accel(ACC_CS);
int         i       = 0;

void setup() {
    pinMode(WKP, INPUT_PULLDOWN);
    
    SPI.begin();                    // Bus initialisation, UC8156 supp. max 10Mhz (writing)
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));//and 6.6Mhz (reading)
 
    display.begin();
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
    display.print("Please tap to increase  the counter!");
    display.update(); 
  
    accel.begin();
    accel.activateTapOnInt1();
}

void loop() {
    if (digitalRead(WKP)==HIGH) {
        i+=1;
        display.fillRect(0, 4, 70, 43, EPD_WHITE);
        display.setTextSize(6);
        display.setCursor(0,4);
        display.print(i);
        display.update(EPD_UPD_MONO);
        accel.clearLatchedInt1();
    }
}
