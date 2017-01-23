/*********************************************************************
This is a library for Plasticlogic's Eink-based Electrophoretic 
Epaper Display (EPD or "ePaper") based on a UC8156 driver.

This displays uses 4 wire SPI to communicate.

Plasticlogic invests time and resources providing this open source code, 
please support us and open-source hardware by purchasing products from 
Plasticlogic & Adafruit!

Written by Robert Poser for Plasticlogic, Dresden/Germany.  
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/
#include "BO_BMA250.h"

BO_BMA250::BO_BMA250(uint8_t _cs2) {
    cs2     = _cs2;     //BMA250 accelerometer
}

void BO_BMA250::initializeBMA250() {
    pinMode(cs2, OUTPUT);
    digitalWrite(cs2, LOW);
    SPI.transfer(0x0F);			//set g
    SPI.transfer(ACC_GSEL);
    digitalWrite(cs2, HIGH);
    digitalWrite(cs2, LOW);
    SPI.transfer(0x10);			//set bandwith
    SPI.transfer(ACC_BW);
    digitalWrite(cs2, HIGH);
}

void BO_BMA250::activateTapOnInt1() {
    digitalWrite(cs2, LOW);
    SPI.transfer(0x16);			//enable interrupt
    SPI.transfer(0x20);
    digitalWrite(cs2, HIGH);
    digitalWrite(cs2, LOW);
    SPI.transfer(0x19);			//enable interrupt
    SPI.transfer(0x20);
    digitalWrite(cs2, HIGH);
    
    digitalWrite(cs2, LOW);
    SPI.transfer(0x2B);			//adjust tab sensitivy (1..1F, the lower the more sensitive)
    SPI.transfer(0x01);
    digitalWrite(cs2, HIGH);
    
    digitalWrite(cs2, LOW);
    SPI.transfer(0x21);			//interrupt mode = temporary, 1s
    SPI.transfer(0x03);
    digitalWrite(cs2, HIGH);
}

void BO_BMA250::readAccel() {
    digitalWrite(cs2, LOW);
    SPI.transfer(0x02|0x80); 
    x = SPI.transfer(0xFF);
    x |= SPI.transfer(0xFF) << 8;
    x >>= 6;
    y = SPI.transfer(0xFF);
    y |= SPI.transfer(0xFF) << 8;
    y >>= 6;
    z = SPI.transfer(0xFF);
    z |= SPI.transfer(0xFF) << 8;
    z >>= 6;
    temp = SPI.transfer(0xFF);
    digitalWrite(cs2, HIGH);
}