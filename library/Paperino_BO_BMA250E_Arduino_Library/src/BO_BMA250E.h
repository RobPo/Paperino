/* *****************************************************************************************
BO_BMA250E.H - A Hardware Library for the Accelerometer BMA250E from BOSCH. The device 
communicates via 4-wire SPI.

Created by Robert Poser, July 19th 2017, Dresden/Germany
Released under BSD license (3-clause BSD license), check license.txt for more informations.

We invested time and resources providing this open source code, please support Paperino and 
open source hardware by purchasing this product @Crowdsupply @Paperino @Plasticlogic
***************************************************************************************** */
#ifndef _BO_BMA250E_h
#define _BO_BMA250E_h

#include "Adafruit_GFX.h"
#include "PL_microEPD.h"

#define ACC_GSEL    0x03    //0x03 - 2g, 0x05 - 4, 0x08 - 8g, 0x0C - 16g
#define ACC_BW      0x08    //7.81Hz bandwith

class BO_BMA250 {
    
    public:
        BO_BMA250(uint8_t _cs2);
        void begin(void);
        void activateTapOnInt1(void);
        void readAccel(void);
    
    private:
        int x, y, z, temp;
        int cs2;
};

#endif