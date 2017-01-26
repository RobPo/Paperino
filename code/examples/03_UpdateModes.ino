// PAPERINO: Full updates vs. partial update demo

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

    display.setTextSize(2);
    display.print("Update Modes");          //Writes content into image buffer
    display.updateFull();                   //Triggers a full image update, showing content of the imagebuffer
    delay(2000);
    display.setTextSize(1);
    display.print("This is a ");
    display.setTextColor(3, 0);
    display.println("full update.");
    display.updateFull();                   //Triggers a full image update, showing content of the imagebuffer
    delay(2000);
    display.setTextColor(0, 3);
    display.println("All pixels are reset no matter whether they are changing or not.");
    display.updateFull();
    delay(3000);
    display.println("Hm, a bit boring, isn't it...?");
    display.updateFull();

    delay(5000);
    display.clear();
    display.setCursor(1,4);
    display.updateFull();                   //Triggers a full image update, showing content of the imagebuffer
    display.setTextSize(2);
    display.println("OK!");                 //Writes content into image buffer
    display.updatePartial();                //Triggers a full image update, showing content of the imagebuffer
    delay(2000);
    display.setTextSize(1);
    display.print("Now you can see ");
    display.setTextColor(3, 0);
    display.print("partial updates!");
    display.updatePartial();                //Triggers a full image update, showing content of the imagebuffer
    delay(2000);
    display.setTextColor(0, 3);
    display.print(" Only changing  pixels are updated. ");
    display.updatePartial();
    delay(2000);
    display.print("Thisgenerates less flicker  and results in lower    power consumption. ");
    display.updatePartial();

}

void loop() {
}