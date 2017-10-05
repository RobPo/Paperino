/* ****************************************************************************************
   Example #4: Demo how to use full, partial and mono updates.
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

    display.begin();
    display.setTextSize(2);
    display.println("Update Modes");
    display.setTextSize(1);
    display.println("-Full Update");
    display.println("-Partial Update");
    display.println("-Partial Mono Update");
    display.update();                       // Triggers a Full update, 4 GL, 800ms

    delay(4000);
    display.clear();
    display.setTextSize(2);
    display.print("EPD_UPD_FULL");
    display.update();                       // Triggers a Full update, 4 GL, 800ms
    delay(1000);
    display.setTextSize(1);
    display.setCursor(1, 20);
    display.print("This is a ");
    display.setTextColor(EPD_WHITE, EPD_BLACK);
    display.println("full update.");
    display.update();
    delay(1000);
    display.setTextColor(EPD_BLACK, EPD_WHITE);
    display.println("All pixels are reset no matter whether they are changing or not.");
    display.update();
    delay(2000);
    display.println("Hm, a bit boring, isn't it...?");
    display.update();

    delay(4000);
    display.clear();
    display.setTextSize(2);
    display.print("EPD_UPD_PART");
    display.update(EPD_UPD_PART);           // Triggers a Partial update, 4 GL, 800ms
    delay(1000);
    display.setTextSize(1);
    display.setCursor(1, 20);
    display.print("OK! Now you see ");
    display.setTextColor(EPD_WHITE, EPD_BLACK);
    display.print("partial updates!");
    display.update(EPD_UPD_PART);
    delay(1000);
    display.setTextColor(EPD_BLACK, EPD_WHITE);
    display.print(" Only changing  pixels are updated. ");
    display.update(EPD_UPD_PART);
    delay(1000);
    display.print("Thisgenerates less flicker  and results in lower    power consumption. ");
    display.update(EPD_UPD_PART);

    delay(4000);
    display.clear();
    display.setTextSize(2);
    display.print("EPD_UPD_MONO");
    display.update(EPD_UPD_MONO);           // Triggers a Partial Mono update, 2 GL, 250ms
    delay(1000);
    display.setTextSize(1);
    display.setCursor(1, 18);
    display.setTextColor(EPD_WHITE, EPD_BLACK);
    display.print("Mono updates");
    display.setTextColor(EPD_BLACK, EPD_WHITE);
    display.print(" allow a bitfaster update rates ");
    display.update(EPD_UPD_MONO);
    delay(1000);
    display.print("(butsupport only two grey-  levels).");
    display.update(EPD_UPD_MONO);

    delay(1000);
    display.drawRect(9, 53, 125, 14, EPD_BLACK);
    display.update(EPD_UPD_MONO);
    for (int i=1; i<18; i++) {
        display.fillRect(6 + 7*i, 55, 5, 10, EPD_BLACK);
        display.update(EPD_UPD_MONO);
    }
}

void loop() {
}
