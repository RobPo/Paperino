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
#include "PLD_tinyEPD.h"

byte buffer[EPD_BUFFER_SIZE];	//the memory buffer for the 1.1" Plasticlogic EPD

PLD_tinyEPD::PLD_tinyEPD(uint8_t _cs, uint8_t _cs2, uint8_t _rst, uint8_t _busy) : Adafruit_GFX(EPD_WIDTH, EPD_HEIGHT) {
    cs      = _cs;      //UC8156 display controller
    cs2     = _cs2;     //BMA250 accelerometer
    rst     = _rst;
    busy    = _busy;
}

void PLD_tinyEPD::begin(bool erase) {
    pinMode(cs, OUTPUT);
    pinMode(rst, OUTPUT);
    pinMode(busy, INPUT);
    digitalWrite(rst, HIGH);
    waitForBusyInactive();
    digitalWrite(rst, LOW);
    waitForBusyInactive();
    digitalWrite(rst, HIGH);
    waitForBusyInactive();
    writeRegister(EPD_PANELSETTING, 0x12, -1, -1, -1);      //Panel setting: Selected gate output as VGL; non-selected as VGH; Gate-Scan G0..G159; Source-Scan S0..S239; Source driver order: Interlaced
    writeRegister(EPD_DRIVERVOLTAGE, 0x25, 0xff, -1, -1);	//Driver voltage setting: Set VGH=+17V & VGL=-25V, VSH=+15V & VSL=-15V
    //writeRegister(EPD_WRITEPXRECTSET, 0, 179, 60, 159);	//Panel resolution setting: 0, SL_max-1, GL_max-GL, GL_max-1 --> 1.4"
    writeRegister(EPD_WRITEPXRECTSET, 0, 71, 0, 147);		//Panel resolution setting: 0, SL_max-1, GL_max-GL, GL_max-1 --> 1.1"
    writeRegister(EPD_VCOMCONFIG, 0x00, 0x00, 0x24, 0x07);	//VCOM configuration: BPCOM=GND; TPCOM=GND after update; Gate=GND after update
    writeRegister(EPD_BORDERSETTING, 0x04, -1, -1, -1);		//VBORDER: GS0-2-GS0 & VBORDER = GND during non-update time
    writeRegister(EPD_PROGRAMMTP, 0xF3, -1, -1, -1); 
    writeRegister(EPD_LOADMONOWF, 0x60, -1, -1, -1);		//Mono-WF laden
	writeRegister(EPD_INTTEMPERATURE, 0x0A, -1, -1, -1);
	if (erase)
		TrippleWhiteErase();
}


void PLD_tinyEPD::drawPixel(int16_t x, int16_t y, uint16_t color) {
    //if ((x < 0) || (x >= EPD_WIDTH || (y < 0) || (y >= EPD_HEIGHT))) return;  //need to comment this temporarely out to enable 1.1" support
    
	if (color < 4) {
		uint16_t byteIndex = x/4 + (y) * nextline;
		uint8_t pixels = buffer[byteIndex];
				
        switch (x%4) {	//2-bit grayscale dot
			case 0: pixels = (pixels & 0x3F) | ((uint8_t)color << 6); break;	//00xx xxxx aka bxxx
			case 1: pixels = (pixels & 0xCF) | ((uint8_t)color << 4); break;	//xx00 xxxx aka xbxx
			case 2: pixels = (pixels & 0xF3) | ((uint8_t)color << 2); break;	//xxxx xx00 aka xxbx 
			case 3: pixels = (pixels & 0xFC) | (uint8_t)color; break;			//xxxx xx00 aka xxxb
			}
		buffer[byteIndex] = pixels;
	}
}

void PLD_tinyEPD::setRotation(uint8_t o) {
    clear();
    if (o==1) {
        nextline = EPD_HEIGHT/4;		//landscape mode (default)
        writeRegister(EPD_DATENTRYMODE, 0x07, -1, -1, -1);  
        _width  = HEIGHT;
        _height = WIDTH;
    }
    if (o==2) {
        nextline = EPD_WIDTH/4;			//portrait mode 
        writeRegister(EPD_DATENTRYMODE, 0x02, -1, -1, -1);  
        _width  = WIDTH;
        _height = HEIGHT;
	}
}

void PLD_tinyEPD::powerOn() {
    waitForBusyInactive();
    //writeRegister(EPD_SETRESOLUTION, 0, 239, 0, 159);			//Panel resolution setting: 0, SL_max-1, GL_max-GL, GL_max-1 --> 1.4"
    writeRegister(EPD_SETRESOLUTION, 0, 239, 0, 147);			//Panel resolution setting: 0, SL_max-1, GL_max-GL, GL_max-1 --> 1.1"
    writeRegister(EPD_TCOMTIMING, 0x67, 0x55, -1, -1);			//Gate active periode (103µs) & source2gate (10µs) and gate2source (10µs)
    writeRegister(EPD_POWERSEQUENCE, 0x00, 0x00, 0x00, -1);		//Power sequence: G2S & S2G voltage on = no delay & null frame periode = no delay
    writeRegister(EPD_POWERCONTROL, 0xD1, -1, -1, -1);
    waitForBusyInactive();
    delay(100);
}

void PLD_tinyEPD::clear() {
	memset(buffer, 0xFF, arraySize(buffer));
    writeBuffer();
}

void PLD_tinyEPD::powerOff() {
    writeRegister(EPD_POWERCONTROL, 0xD0, -1, -1, -1);
    waitForBusyInactive();
    writeRegister(EPD_POWERCONTROL, 0xC0, -1, -1, -1);
}


void PLD_tinyEPD::writeBuffer(){
    //writeRegister(EPD_PIXELACESSPOS, 0, 159, -1, -1);			//Pixel Access position: X-Start-Address=0; Y-Start-Address=GL-1 --> 1.4"
    writeRegister(EPD_PIXELACESSPOS, 0, 147, -1, -1);			//Pixel Access position: X-Start-Address=0; Y-Start-Address=GL-1 --> 1.1"
    digitalWrite(cs, LOW);
    SPI.transfer(0x10);
	SPI.transfer(buffer, NULL, arraySize(buffer), NULL);
    digitalWrite(cs, HIGH);
    waitForBusyInactive();
}


uint8_t PLD_tinyEPD::readTemperature() {
	uint8_t temp;
    digitalWrite(cs, LOW);                                      //Read current temperature value 
    SPI.transfer(EPD_REGREAD | 0x08);
	temp = SPI.transfer(0xFF);
    digitalWrite(cs, HIGH);
	waitForBusyInactive();
	return temp;
}

void PLD_tinyEPD::updateFull() {
    writeBuffer();
    powerOn();
    writeRegister(EPD_DISPLAYENGINE, 0x03, -1, -1, -1);    
    waitForBusyInactive();
    powerOff();
}

void PLD_tinyEPD::updatePartial() {
    writeBuffer();
    powerOn();
    writeRegister(EPD_DISPLAYENGINE, 0x07, -1, -1, -1);    
    waitForBusyInactive();
    powerOff();
}

void PLD_tinyEPD::FastupdatePartial() {
    writeBuffer();
    powerOn();
    writeRegister(EPD_PROGRAMMTP, 0x02, -1, -1, -1); 
    writeRegister(EPD_LOADMONOWF, 0x60, -1, -1, -1);			//Mono-WF loading
    writeRegister(EPD_DISPLAYENGINE, 0x07, -1, -1, -1);    
    waitForBusyInactive();
    powerOff();
}

void PLD_tinyEPD::writeRegister(char address, char val1, char val2, char val3, char val4) {
    digitalWrite(cs, LOW);
    SPI.transfer(address);
    SPI.transfer(val1);
    if (val2>-1) SPI.transfer(val2);
    if (val3>-1) SPI.transfer(val3);
    if (val4>-1) SPI.transfer(val4);
    digitalWrite(cs, HIGH);
    waitForBusyInactive();
}

void PLD_tinyEPD::TrippleWhiteErase() {
    clear();
    updateFull();
    invertDisplay();
    updateFull();
    clear();
    updateFull();
}

void PLD_tinyEPD::invertDisplay() {
    for (int i=0; i<arraySize(buffer); i++) {
        buffer[i] = ~buffer[i];
	}
	SPI.transfer(buffer, NULL, arraySize(buffer), NULL);
}

void PLD_tinyEPD::waitForBusyInactive(){
    while (digitalRead(busy) == LOW) {Particle.process();}
}


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