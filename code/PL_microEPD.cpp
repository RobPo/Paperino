#include "PL_microEPD.h"

byte buffer[EPD_BUFFER_SIZE];	//the memory buffer for the 1.1" Plasticlogic EPD

PL_microEPD::PL_microEPD(uint8_t _cs, uint8_t _rst, uint8_t _busy) : Adafruit_GFX(EPD_WIDTH, EPD_HEIGHT) {
    cs      = _cs;      //UC8156 display controller
    rst     = _rst;
    busy    = _busy;
}

void PL_microEPD::begin(bool whiteErase) {
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
	if (whiteErase) WhiteErase();
	setTextColor(EPD_BLACK);
	setRotation(1);
}


void PL_microEPD::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= _width || (y < 0) || (y >= _height))) return;  
    
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

void PL_microEPD::setRotation(uint8_t o) {
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

void PL_microEPD::powerOn() {
    waitForBusyInactive();
    //writeRegister(EPD_SETRESOLUTION, 0, 239, 0, 159);			//Panel resolution setting: 0, SL_max-1, GL_max-GL, GL_max-1 --> 1.4"
    writeRegister(EPD_SETRESOLUTION, 0, 239, 0, 147);			//Panel resolution setting: 0, SL_max-1, GL_max-GL, GL_max-1 --> 1.1"
    writeRegister(EPD_TCOMTIMING, 0x67, 0x55, -1, -1);			//Gate active periode (103µs) & source2gate (10µs) and gate2source (10µs)
    writeRegister(EPD_POWERSEQUENCE, 0x00, 0x00, 0x00, -1);		//Power sequence: G2S & S2G voltage on = no delay & null frame periode = no delay
    writeRegister(EPD_POWERCONTROL, 0xD1, -1, -1, -1);
    waitForBusyInactive();
    delay(100);
}

void PL_microEPD::clear() {
	memset(buffer, 0xFF, arraySize(buffer));
    writeBuffer();
    setCursor(0,0);
}

void PL_microEPD::powerOff() {
    writeRegister(EPD_POWERCONTROL, 0xD0, -1, -1, -1);
    waitForBusyInactive();
    writeRegister(EPD_POWERCONTROL, 0xC0, -1, -1, -1);
}


void PL_microEPD::writeBuffer(){
    //writeRegister(EPD_PIXELACESSPOS, 0, 159, -1, -1);			//Pixel Access position: X-Start-Address=0; Y-Start-Address=GL-1 --> 1.4"
    writeRegister(EPD_PIXELACESSPOS, 0, 147, -1, -1);			//Pixel Access position: X-Start-Address=0; Y-Start-Address=GL-1 --> 1.1"
    digitalWrite(cs, LOW);
    SPI1.transfer(0x10);
	SPI1.transfer(buffer, NULL, arraySize(buffer), NULL);
    digitalWrite(cs, HIGH);
    waitForBusyInactive();
}


uint8_t PL_microEPD::readTemperature() {
	uint8_t temp;
    digitalWrite(cs, LOW);                                      //Read current temperature value 
    SPI1.transfer(EPD_REGREAD | 0x08);
	temp = SPI1.transfer(0xFF);
    digitalWrite(cs, HIGH);
	waitForBusyInactive();
	return temp;
}

void PL_microEPD::updateFull() {
    writeBuffer();
    powerOn();
    writeRegister(EPD_DISPLAYENGINE, 0x03, -1, -1, -1);    
    waitForBusyInactive();
    powerOff();
}

void PL_microEPD::updatePartial() {
    writeBuffer();
    powerOn();
    writeRegister(EPD_DISPLAYENGINE, 0x07, -1, -1, -1);    
    waitForBusyInactive();
    powerOff();
}

void PL_microEPD::FastupdatePartial() {
    writeBuffer();
    powerOn();
    writeRegister(EPD_PROGRAMMTP, 0x02, -1, -1, -1); 
    writeRegister(EPD_LOADMONOWF, 0x60, -1, -1, -1);			//Mono-WF loading
    writeRegister(EPD_DISPLAYENGINE, 0x07, -1, -1, -1);    
    waitForBusyInactive();
    powerOff();
}

void PL_microEPD::writeRegister(char address, char val1, char val2, char val3, char val4) {
    digitalWrite(cs, LOW);
    SPI1.transfer(address);
    SPI1.transfer(val1);
    if (val2>-1) SPI1.transfer(val2);
    if (val3>-1) SPI1.transfer(val3);
    if (val4>-1) SPI1.transfer(val4);
    digitalWrite(cs, HIGH);
    waitForBusyInactive();
}

void PL_microEPD::WhiteErase() {
    clear();
    updateFull();
    invertDisplay();
    updateFull();
    clear();
    updateFull();
}

void PL_microEPD::invertDisplay() {
    for (int i=0; i<arraySize(buffer); i++) {
        buffer[i] = ~buffer[i];
	}
	SPI1.transfer(buffer, NULL, arraySize(buffer), NULL);
}

void PL_microEPD::waitForBusyInactive(){
    while (digitalRead(busy) == LOW) {Particle.process();}
}


