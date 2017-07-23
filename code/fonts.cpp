//***************************************************************************
//This is an example sketch for PLASTICLOGIC's EPD Displays in combination
//with a accelerometer (BMA250). Its tap detection is used here to wake-up
//the Particle Photon.
//***************************************************************************

//SYSTEM_MODE(SEMI_AUTOMATIC);
//STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

#define EPD_RST     A0
#define EPD_BUSY    A1
#define EPD_CS      D5

#include "Adafruit_mfGFX.h"
#include "PL_microEPD.h"

PL_microEPD display(EPD_CS, EPD_RST, EPD_BUSY);  
int a = -1;

void setup() {
    // Subscribe to the integration response event
    Particle.subscribe("hook-response/gmail_count", myHandler, MY_DEVICES);
    
    // Initialize SPI
    SPI1.begin();
    SPI1.setBitOrder(MSBFIRST);                 
    SPI1.setDataMode(SPI_MODE0); 
    SPI1.setClockDivider(SPI_CLOCK_DIV4);
  
    // Initialize Paperino
    display.begin(true);                    //Initializing. TRUE=Initial update to refresh the screen to defined state
}

void myHandler(const char *event, const char *data) {
    int c = atoi(data);
    display.clear();
    if (c == 0) {
        if (a > 0 || a < 0 ) {
            // DRAW lucky smily :-)
            display.drawCircle(75,82,70,0);
            display.drawCircle(75,83,70,0);
            display.drawCircle(75,84,70,0);
            display.drawCircle(75,85,70,0);
            display.drawCircle(75,86,70,0);
            display.drawCircle(75,87,70,0);
            display.drawCircle(75,88,70,0);
            display.drawCircle(75,89,70,0);
            display.fillRect(0,0,15,72,3);
            display.fillRect(122,0,26,72,3);
            display.fillTriangle(10,47,20,42,15,42,0);
            display.fillTriangle(112,32,130,37,122,40,0);
            display.setCursor(70,65);
            display.setTextSize(1);    
            display.println(c);
            display.updateFull();
            a = 0;
        }
    } else {
        if (c > a || c < a) {
            // DRAW sad smily :-(
            display.drawCircle(75,-10,70,0);
            display.drawCircle(75,-11,70,0);
            display.drawCircle(75,-12,70,0);
            display.drawCircle(75,-13,70,0);
            display.drawCircle(75,-14,70,0);
            display.drawCircle(75,-15,70,0);
            display.drawCircle(75,-16,70,0);
            display.drawCircle(75,-17,70,0);
            display.fillRect(0,0,15,72,3);
            display.fillRect(122,0,26,72,3);
            display.fillTriangle(10,17,20,12,15,22,0);
            display.fillTriangle(112,32,130,37,122,40,0);
            display.setCursor(70,65);
            display.setTextSize(1);    
            display.println(c);
            display.updateFull();
            a = c;
        }
    }
}


void loop() {
  String data = String(9);
  Particle.publish("gmail_count", data, PRIVATE);
  delay(60000);
}
