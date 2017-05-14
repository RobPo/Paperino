// *****************Traffic Indicator Demo *************************************************
// Project that helps you calculate the ETA (estimated time arrival) between to fixed 
// points (origins,destination), based on a webhook towards the Google Maps API, 
// returning back the time according to the actual traffic conditions. The demo is based on 
// Eslam's nice project @Hackaday.io (https://hackaday.io/project/16096-display-traffic-
// status-using-arduino-1sheeld) and evolved to display the time on a Paperino ePaper screen.
//
// IF you like this project please support https://www.crowdsupply.com/robert-poser/paperino
// (c) 2017 Robert Poser
// *****************************************************************************************
#include "Adafruit_mfGFX.h"
#include "PL_microEPD.h"

#define EPD_RST     A0
#define EPD_BUSY    A1
#define EPD_CS      D5

PL_microEPD display(EPD_CS, EPD_RST, EPD_BUSY);  

void setup() {
    Time.zone(+2);          // Please adjust time zone according to your location
                            // Subscription to a webhook event "TrafficIndicator"
    Particle.subscribe("hook-response/TrafficIndicator", myHandler, MY_DEVICES);
    SPI1.begin();           // Initialize SPI
    SPI1.beginTransaction(SPISettings(8*MHZ, MSBFIRST, SPI_MODE0));  
    display.begin(true);    // Initialize Paperino ePaper and refresh screen once
}
                            // This function is called once the webhook data arrived
void myHandler(const char *event, const char *data) {
    String wh_result = data;
    String tempStr;
    
    display.setTextSize(1);
    display.setCursor(2,4);
    display.print("From: ");
    tempStr = wh_result.substring(0, wh_result.indexOf(","));
    if(tempStr.length()>17) tempStr = tempStr.substring(0,19);
    display.println(tempStr);
    display.setCursor(2,12);
    display.print("To:   ");
    tempStr = wh_result.substring((wh_result.indexOf("~"))+1, wh_result.indexOf(",", wh_result.indexOf("~")+1));
    display.println(tempStr.substring(0,17));
    tempStr = wh_result.substring((wh_result.indexOf("~"))+1, wh_result.length());
    display.setCursor(2,20);
    display.print("Dist: ");
    tempStr = tempStr.substring((tempStr.indexOf("~"))+1, tempStr.length());
    display.print(tempStr.substring(0,tempStr.indexOf("~")));
    display.print("  ");
    display.print(Time.hour());
    display.print(":");
    display.print(Time.minute());
    display.println(" Uhr");
    tempStr = tempStr.substring((tempStr.indexOf("~"))+1, tempStr.length());
    display.setCursor(0,35);
    display.setTextSize(5);
    display.println(tempStr.substring(0,(tempStr.indexOf(" "))+1));
    tempStr = tempStr.substring((tempStr.indexOf(" "))+1, tempStr.length());
    display.setCursor(65,49);
    display.setTextSize(3);
    display.println(tempStr.substring(0,20));
    display.updateFull();
    SPI1.endTransaction();
}


void loop() {               // Publish webhook TrafficIndicator every five minutes
  Particle.publish("TrafficIndicator");
  delay(10000);              // Time to go to deep sleep
  System.sleep(SLEEP_MODE_DEEP,5*60);
}
