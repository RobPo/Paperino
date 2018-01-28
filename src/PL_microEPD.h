/* *****************************************************************************************
PL_MICROEPD.H - A Hardware Library for 1.1” E-Paper display (EPD) from Plastic Logic based
on UC8156 IC. This display uses six GPIOs to communicate (4-wire SPI, reset pin, busy pin).

Created by Robert Poser, Oct 5th 2017, Dresden/Germany. Released under BSD license
(3-clause BSD license), check license.md for more information.

We invested time and resources providing this open source code, please support Paperino and
open source hardware by purchasing this product @Crowdsupply @Watterott @Plasticlogic
***************************************************************************************** */
#ifndef PL_microEPD_h
#define PL_microEPD_h

#include <Adafruit_GFX.h>               //Only if Arduino attached
#ifdef ARDUINO
#include <SPI.h>
#elif defined(PLATFORM_ID)              //Only if a Particle attached
#endif

#define EPD_WIDTH   (72)
#define EPD_HEIGHT  (148)
#define EPD_BUFFER_SIZE    EPD_WIDTH * EPD_HEIGHT / 4      //2 bits per pixel

#define EPD_BLACK 0x00
#define EPD_DGRAY 0x01
#define EPD_LGRAY 0x02
#define EPD_WHITE 0x03

#define EPD_UPD_FULL  0x00            // Triggers a Full update, 4 GL, 800ms
#define EPD_UPD_PART  0x01            // Triggers a Partial update, 4 GL, 800ms
#define EPD_UPD_MONO  0x02            // Triggers a Partial Mono update, 2 GL, 250ms

#define EPD_TMG_LNG     880             // Duration{ms} of a full update
#define EPD_TMG_MID     340             // Duration{ms} of a partial update
#define EPD_TMG_SRT     1
#define EPD_TMG_SR2     70

#define EPD_REVISION          0x00  // Revision, Read only
#define EPD_PANELSETTING      0x01
#define EPD_DRIVERVOLTAGE     0x02
#define EPD_POWERCONTROL      0x03
#define EPD_TCOMTIMING        0x06
#define EPD_INTTEMPERATURE    0x07
#define EPD_SETRESOLUTION     0x0C
#define EPD_WRITEPXRECTSET    0x0D
#define EPD_PIXELACESSPOS     0x0E
#define EPD_DATENTRYMODE      0x0F
#define EPD_DISPLAYENGINE     0x14
#define EPD_VCOMCONFIG        0x18
#define EPD_BORDERSETTING     0x1D
#define EPD_POWERSEQUENCE     0x1F
#define EPD_PROGRAMMTP        0x40
#define EPD_LOADMONOWF        0x44
#define EPD_REGREAD           0x80  // Instruction R/W bit set HIGH for data READ

#define ACC_GSEL    0x03    // Range: 0x03 - +/-2g, 0x05 - +/-4g, 0x08 - +/-8g, 0x0C - +/-16g
#define ACC_BW      0x0F    // Bandwidth: 0x08 = 7.81Hz bandwith, 0x0F = 1000Hz

class PL_microEPD : public Adafruit_GFX {

public:
    PL_microEPD(uint8_t _cs, int _rst=-1, int _busy=-1);
    void begin(bool erase=true);
    void clear();
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void invert(void);
    void update(int updateMode=EPD_UPD_FULL);
    void setRotation(uint8_t o);
    void scrollText(String text, int cutout=24);
    void RSVPText(String text);
    void setVBorderColor(int color);
    uint8_t readTemperature(void);


private:
    int cs, rst, busy;
    int cursorX, cursorY;
    int fontHeight=8, fontWidth=5;
    int nextline=EPD_WIDTH/4;
    void waitForBusyInactive(int duration);
    void writeRegister(char address, char val1, char val2, char val3, char val4);
    void writeBuffer(void);
    void WhiteErase(void);
    void powerOn(void);
    void powerOff(void);
};


class BO_BMA250 {

    public:
        BO_BMA250(uint8_t _cs2);
        void begin(void);
        void activateTapOnInt1(void);
  void clearLatchedInt1(void);
        void readAccel(void);

    private:
        int x, y, z, temp;
        int cs2;
};

#endif/* *****************************************************************************************
PL_MICROEPD.H - A Hardware Library for 1.1” E-Paper display (EPD) from Plastic Logic based
on UC8156 IC. This display uses six GPIOs to communicate (4-wire SPI, reset pin, busy pin).
Created by Robert Poser, May 18th 2017, Dresden/Germany
Released under BSD license (3-clause BSD license), check license.txt for more informations.
We invested time and resources providing this open source code, please support Paperino and
open source hardware by purchasing this product @Crowdsupply @Paperino @Plasticlogic
***************************************************************************************** */

#ifndef PL_microEPD_h
#define PL_microEPD_h

#include <Adafruit_GFX.h>
#include <SPI.h>

#define EPD_WIDTH   (72)
#define EPD_HEIGHT  (148)
#define EPD_BUFFER_SIZE    EPD_WIDTH * EPD_HEIGHT / 4      //2 bits per pixel

#define EPD_BLACK 0x00
#define EPD_DGRAY 0x01
#define EPD_LGRAY 0x02
#define EPD_WHITE 0x03

#define EPD_UPD_FULL  0x00            // Triggers a Full update, 4 GL, 800ms
#define EPD_UPD_PART  0x01            // Triggers a Partial update, 4 GL, 800ms
#define EPD_UPD_MONO  0x02            // Triggers a Partial Mono update, 2 GL, 250ms

#define EPD_TMG_LNG     880             // Duration{ms} of a full update
#define EPD_TMG_MID     275             // Duration{ms} of a partial update
#define EPD_TMG_SRT     70              // **critical**
#define EPD_TMG_SR2     70

#define EPD_REVISION          0x00  // Revision, Read only
#define EPD_PANELSETTING      0x01
#define EPD_DRIVERVOLTAGE     0x02
#define EPD_POWERCONTROL      0x03
#define EPD_TCOMTIMING        0x06
#define EPD_INTTEMPERATURE    0x07
#define EPD_SETRESOLUTION     0x0C
#define EPD_WRITEPXRECTSET    0x0D
#define EPD_PIXELACESSPOS     0x0E
#define EPD_DATENTRYMODE      0x0F
#define EPD_DISPLAYENGINE     0x14
#define EPD_VCOMCONFIG        0x18
#define EPD_BORDERSETTING     0x1D
#define EPD_POWERSEQUENCE     0x1F
#define EPD_PROGRAMMTP        0x40
#define EPD_LOADMONOWF        0x44
#define EPD_REGREAD           0x80  // Instruction R/W bit set HIGH for data READ

#define ACC_GSEL    0x03    // Range: 0x03 - +/-2g, 0x05 - +/-4g, 0x08 - +/-8g, 0x0C - +/-16g
#define ACC_BW      0x0F    // Bandwidth: 0x08 = 7.81Hz bandwith, 0x0F = 1000Hz

class PL_microEPD : public Adafruit_GFX {

public:
    PL_microEPD(uint8_t _cs, int _rst=-1, int _busy=-1);
    void begin(bool erase=true);
    void clear();
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void invert(void);
    void update(int updateMode=EPD_UPD_FULL);
    void setRotation(uint8_t o);
    void scrollText(String text, int cutout=24);
    void RSVPText(String text);
    void setVBorderColor(int color);
    uint8_t readTemperature(void);


private:
    int cs, rst, busy;
    int cursorX, cursorY;
    int fontHeight=8, fontWidth=5;
    int nextline=EPD_WIDTH/4;
    void waitForBusyInactive(int duration);
    void writeRegister(char address, char val1, char val2, char val3, char val4);
    void writeBuffer(void);
    void WhiteErase(void);
    void powerOn(void);
    void powerOff(void);
};


class BO_BMA250 {

    public:
        BO_BMA250(uint8_t _cs2);
        void begin(void);
        void activateTapOnInt1(void);
  void clearLatchedInt1(void);
        void readAccel(void);

    private:
        int x, y, z, temp;
        int cs2;
};

#endif
