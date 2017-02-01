#ifndef PL_microEPD_h
#define PL_microEPD_h

#include "Adafruit_mfGFX.h"

#define EPD_WIDTH   (72)
#define EPD_HEIGHT  (148)
//#define EPD_WIDTH   (180)
//#define EPD_HEIGHT  (100)

#define EPD_BLACK	0x00
#define EPD_DGRAY	0x01
#define EPD_LGRAY	0x02
#define EPD_WHITE	0x03

#define EPD_BUFFER_SIZE		EPD_WIDTH * EPD_HEIGHT / 4		// 4 grey levels, 2 bits per pixel

#define EPD_REVISION        0x00		// Revision, Read only
#define EPD_PANELSETTING    0x01
#define EPD_DRIVERVOLTAGE   0x02
#define EPD_POWERCONTROL    0x03
#define EPD_TCOMTIMING      0x06
#define EPD_INTTEMPERATURE  0x07
#define EPD_SETRESOLUTION   0x0C
#define EPD_WRITEPXRECTSET  0x0D
#define EPD_PIXELACESSPOS   0x0E
#define EPD_DATENTRYMODE    0x0F
#define EPD_DISPLAYENGINE   0x14
#define EPD_VCOMCONFIG      0x18
#define EPD_BORDERSETTING   0x1D
#define EPD_POWERSEQUENCE   0x1F
#define EPD_PROGRAMMTP      0x40
#define EPD_LOADMONOWF      0x44

#define EPD_REGREAD			0x80	// Instruction R/W bit set HIGH for data READ

#define ACC_GSEL    0x03    //0x03 - 2g, 0x05 - 4, 0x08 - 8g, 0x0C - 16g
#define ACC_BW      0x08    //7.81Hz bandwith


class PL_microEPD : public Adafruit_GFX {
    
public:
    PL_microEPD(uint8_t _cs, uint8_t _rst, uint8_t _busy);
    void begin(bool erase);
    void clear();
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void invertDisplay(void);
    void updateFull(void);
    void updatePartial(void);
    void FastupdatePartial(void);
    void setRotation(uint8_t o);
	uint8_t readTemperature(void);
 

private:
    int cs, rst, busy;
    int cursorX, cursorY;
    int fontHeight=8, fontWidth=5;
    int nextline=EPD_WIDTH/4;            //Start with landscape mode as default
    void waitForBusyInactive(void);      //Wait loop until BUSY pin is inactive (LOW)
    void writeRegister(char address, char val1, char val2, char val3, char val4);
    void writeBuffer(void);
    void WhiteErase(void);
    void powerOn(void);
    void powerOff(void);
};

#endif
