/* *****************************************************************************************
PL_MICROEPD - A Universal Hardware Library for 1.1”, 1.4", 2.1" and 3.1" E-Paper displays 
(EPDs) from Plastic Logic based on UC8156 driver IC. The communication is SPI-based, for more
information about hook-up and tutorials please check: https://github.com/RobPo/Paperino.

Created by Robert Poser, Feb 9th 2018, Dresden/Germany. Released under BSD license
(3-clause BSD license), check license.md for more information.

We invested time and resources providing this open source code, please support Paperino and
open source hardware by purchasing this product @Crowd_supply @Watterott @Plasticlogic
***************************************************************************************** */
#include "PL_microEPD.h"

PL_microEPD::PL_microEPD(int8_t _cs, int8_t _rst, int8_t _busy) : Adafruit_GFX(EPD_WIDTH, 
EPD_HEIGHT) {

    cs      = _cs;
    rst     = _rst;
    busy    = _busy;
}

// PUBLIC

// ******************************************************************************************
// BEGIN - Resetting UC8156 driver IC and configuring all sorts of behind-the-scenes-settings
// By default (WHITEERASE=TRUE) a clear screen update is triggered once to erase the screen.
// ******************************************************************************************
void PL_microEPD::begin(bool whiteErase) {
    pinMode(cs, OUTPUT);  
    if (busy!=-1)
        pinMode(busy, INPUT);

    if (rst!=-1) {
        pinMode(rst, OUTPUT);                   //Trigger a global hardware reset...
        digitalWrite(rst, HIGH);     
        digitalWrite(rst, LOW);
        digitalWrite(rst, HIGH);
        waitForBusyInactive(EPD_TMG_SRT);
        writeRegister(EPD_SOFTWARERESET, -1, -1, -1, -1);    //... or do software reset if no pin defined
    } else
        writeRegister(EPD_SOFTWARERESET, -1, -1, -1, -1);    //... or do software reset if no pin defined

    _EPDsize=getEPDsize();                                  //Read NVM to determine display size
    switch (_EPDsize) {
        case 11:
            _width=72; _height=148; nextline= _width/4; _buffersize=_width*_height/4;
            width=148; height=72;
            writeRegister(EPD_PANELSETTING, 0x12, -1, -1, -1);      	
            writeRegister(EPD_WRITEPXRECTSET, 0, 71, 0, 147);
            writeRegister(EPD_VCOMCONFIG, 0x00, 0x00, 0x24, 0x07);
            break;
        case 14:
            _width=100; _height=180; nextline= _width/4; _buffersize=_width*_height/4;
            width=180; height=100;
            writeRegister(EPD_PANELSETTING, 0x12, -1, -1, -1);      	
            writeRegister(EPD_WRITEPXRECTSET, 0, 0xB3, 0x3C, 0x9F);
            writeRegister(EPD_VCOMCONFIG, 0x00, 0x00, 0x24, 0x07);
            break;
        case 21:
            _width=146; _height=240; nextline= _width/4; _buffersize=_width*_height/4;
            width=240; height=146;
            writeRegister(EPD_PANELSETTING, 0x10, -1, -1, -1);      	
            writeRegister(EPD_WRITEPXRECTSET, 0, 239, 0, 145);	//147?
            writeRegister(EPD_VCOMCONFIG, 0x00, 0x00, 0x24, 0x07);
            break;
        case 31:
            _width=76; _height=312; nextline= _width/4; _buffersize=_width*_height/4;
            width=312; height=76;
            writeRegister(EPD_PANELSETTING, 0x12, -1, -1, -1);      	
            writeRegister(EPD_WRITEPXRECTSET, 0, 0x97, 0, 0x9b);	
            writeRegister(EPD_VCOMCONFIG, 0x50, 0x01, 0x24, 0x07);
    }
    writeRegister(EPD_DRIVERVOLTAGE, 0x25, 0xff, -1, -1);
    writeRegister(EPD_BORDERSETTING, 0x04, -1, -1, -1);
    writeRegister(EPD_LOADMONOWF, 0x60, -1, -1, -1);
    writeRegister(EPD_INTTEMPERATURE, 0x0A, -1, -1, -1);

    setRotation(1);                             //Set landscape mode as default
    if (whiteErase) WhiteErase();               //Start with a white refresh if TRUE
    setTextColor(EPD_BLACK);                    //Set text color to black as default
}

// ************************************************************************************
// CLEAR - Erases the image buffer and triggers an image update and sets the cursor
// back to the origin coordinates (0,0).
// ************************************************************************************
void PL_microEPD::clear() {
    for (int i=0; i<_buffersize; i++) {
        buffer[i] = 0xff;
        buffer2[i] = 0xff;
    }
    setCursor(0,0);
}

// ************************************************************************************
// DRAWPIXEL - Draws pixel in the memory buffer at position X, Y with the value of the
// parameter color (2 bit value).
// ************************************************************************************
void PL_microEPD::drawPixel(int16_t x, int16_t y, uint16_t color) {

    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height) || (color>4 )) return;  

    if (_EPDsize==11 || _EPDsize==3) 
        y=y+3;
    uint8_t pixels = buffer[x/4 + (y) * nextline];
	switch (x%4) {					            //2-bit grayscale dot
    	case 0: buffer[x/4 + (y) * nextline] = (pixels & 0x3F) | ((uint8_t)color << 6); break;	
    	case 1: buffer[x/4 + (y) * nextline] = (pixels & 0xCF) | ((uint8_t)color << 4); break;	
    	case 2: buffer[x/4 + (y) * nextline] = (pixels & 0xF3) | ((uint8_t)color << 2); break;	
    	case 3: buffer[x/4 + (y) * nextline] = (pixels & 0xFC) | (uint8_t)color; break;		
	}
}

void PL_microEPD::drawPixel2(int x, int y, int color) {
   if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height) || (color>4 )) return;  
    
    uint8_t pixels = buffer2[x/4 + (y) * nextline];
	switch (x%4) {					            //2-bit grayscale dot
    	case 0: buffer2[x/4 + (y) * nextline] = (pixels & 0x3F) | ((uint8_t)color << 6); break;	
    	case 1: buffer2[x/4 + (y) * nextline] = (pixels & 0xCF) | ((uint8_t)color << 4); break;	
    	case 2: buffer2[x/4 + (y) * nextline] = (pixels & 0xF3) | ((uint8_t)color << 2); break;	
    	case 3: buffer2[x/4 + (y) * nextline] = (pixels & 0xFC) | (uint8_t)color; break;		
	}
}

int PL_microEPD::getPixel(int x, int y) {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return 5;  

	uint16_t byteIndex = x/4 + (y) * nextline;
    switch (x%4) {				
    		case 0: return ((unsigned int)(buffer[byteIndex] & 0xC0) >> 6); 
     		case 1: return ((unsigned int)(buffer[byteIndex] & 0x30) >> 4);
    		case 2: return ((unsigned int)(buffer[byteIndex] & 0x0C) >> 2);
    		case 3: return ((unsigned int)(buffer[byteIndex] & 0x03)); 
	}
}

// ************************************************************************************
// INVERT - Inverts the screen content from black to white and vice versa
// ************************************************************************************
void PL_microEPD::invert() {
    for (int i=0; i<_buffersize; i++) 
        buffer[i] = ~buffer[i];
}

// ************************************************************************************
// UPDATE - Triggers an image update based on the content written in the image buffer.
// There are three different updateModes supported: EPD_UPD_FULL(0) is set by default,
// achieves four greyelevels, takes about 800ms and refreshes all pixels. This is the
// update mode having the best image quality. EPD_UPD_PART(1) is a variant of the
// previous one but only changing pixels are refreshed. This results in less flickering
// for the price of a slightly higher pixel to pixel crosstalk. EPD_UPD_MONO(2) is
// again a variant of the previous update mode but only about 250ms long. this allows
// slightly faster and more responsive updates for the price of only two greylevels
// being supported (EPD_BLACK and EPD_WHITE). Depending on your application it is
// recommended to insert a full update EPD_UPD_FULL(0) after a couple of mono updates
// to increase the image quality.
// THIS KIND OF DISPLAY IS NOT SUITED FOR LONG RUNNING ANIMATIONS OR APPLICATIONS WITH
// CONTINUOUSLY HIGH UPDATE RATES. AS A RULE OF THUMB PLEASE TRIGGER UPDATES IN AVERAGE
// NOT FASTER THAN MINUTELY (OR RUN BACK2BACK UPDATES NOT LONGER AS ONE HOUR PER DAY.)
// ************************************************************************************
void PL_microEPD::update(int updateMode) {
    scrambleBuffer();
    writeBuffer();
    powerOn();
    switch (updateMode) {
        case 0:
            writeRegister(EPD_PROGRAMMTP, 0x00, -1, -1, -1);
            writeRegister(EPD_DISPLAYENGINE, 0x03, -1, -1, -1);
            waitForBusyInactive(EPD_TMG_LNG);
            break;
        case 1:
            writeRegister(EPD_PROGRAMMTP, 0x00, -1, -1, -1);
            writeRegister(EPD_DISPLAYENGINE, 0x07, -1, -1, -1);
            waitForBusyInactive(EPD_TMG_LNG);
            break;
        case 2:
            writeRegister(EPD_PROGRAMMTP, 0x02, -1, -1, -1);
            writeRegister(EPD_DISPLAYENGINE, 0x07, -1, -1, -1);
            waitForBusyInactive(EPD_TMG_MID);
    }
    powerOff();
}

// ************************************************************************************
// SCRAMBLEBUFFER - 
// ************************************************************************************
void PL_microEPD::scrambleBuffer() {
    switch (_EPDsize) {
        case 21:
            for (int y=0; y<146; y++) {                   // for each gateline...
                for (int x=0; x<240/2; x++) {             // for each sourceline...
                    drawPixel(239-x, y, getPixel(x,y+1));
                    drawPixel(x, y, getPixel(x+120,y+1));
                }
            }
            break;
        case 31:
            for (int y=0; y<_height; y++) {               // for each gateline...
                //scrambleline=
                for (int x=0; x<_width; x++) {            // for each sourceline...
                    if (x%2)                                    // if x = 1, 3, 5..
                        drawPixel2((x-1)/2, y+1, getPixel(x,y));
                    else
                        drawPixel2(_width/2+x/2, y, getPixel(x,y));
                }
            }
    }
}

// ************************************************************************************
// SETROTATION - Let’s you define the display orientation. If set to “1” the landscape
// mode is select (default), if set to “2” the display is set to portrait mode.
// ************************************************************************************
void PL_microEPD::setRotation(uint8_t o) {
    clear();
    if (o==1) {
        nextline = _height/4;      				//Landscape mode (default)
        switch (_EPDsize) {
            case 11:
                writeRegister(EPD_DATENTRYMODE, 0x07, -1, -1, -1); 
                break;
            case 14:
                writeRegister(EPD_DATENTRYMODE, 0x02, -1, -1, -1); 
                break;
            case 21:
                writeRegister(EPD_DATENTRYMODE, 0x20, -1, -1, -1);  
                break;
            case 31:
                writeRegister(EPD_DATENTRYMODE, 0x07, -1, -1, -1);  
        }
        
        _width  = _width + _height;
        _height = _width - _height;
        _width  = _width - _height;
    }
    if (o==2) {
        nextline = _width/4;     //Portrait mode
        writeRegister(EPD_DATENTRYMODE, 0x02, -1, -1, -1);
        _width  = _width;
        _height = _height;
  }
}

// ************************************************************************************
// SCROLLTEXT - Function to scroll a String (text) across the screen for a given amount
// of loops and String extract length
// ************************************************************************************
void PL_microEPD::scrollText(String text, int cutout) {
    int temp_x = cursor_x;
    int temp_y = cursor_y;
    setTextWrap(false);

    for (unsigned int offset = 0; offset < text.length(); offset++) {
        String t = "";
        for (int i = 0; i < cutout; i++)
            t += text.charAt((offset + i));
        setCursor(temp_x, temp_y);
        print(t);
        update(EPD_UPD_MONO);
        clear();
        #if defined(PLATFORM_ID)    // If Particle used, keep cloud connection alive
            Particle.process();
        #endif
    }
}

// ************************************************************************************
// RAPID SERIAl VISUAL PRESENTATION - Dynamic, word-by-word playing of longer strings
// on small screen. About 210 words per minute are possible. Reading speed is constant
// and vowel alignment is not implemented.
// ************************************************************************************
void PL_microEPD::RSVPText(String text) {
    int start=0, end = 1;
    while (end > 0) {
        end = text.indexOf(" ", start);
        clear();
        setCursor(10,20);
        print(text.substring(start, end));
        update(EPD_UPD_MONO);
        start = end + 1;
    }
}

// ************************************************************************************
// SETVBORDERCOLOR - Sets the color of the VBorder around the active area. By default
// this is set to White (matching to the Paperino FrontCover) and should not be changed
// ************************************************************************************
void PL_microEPD::setVBorderColor(int color) {
    if (color==3) writeRegister(EPD_BORDERSETTING, 0xF7, -1, -1, -1);  //
    if (color==0) writeRegister(EPD_BORDERSETTING, 0x07, -1, -1, -1);  //
    update(EPD_UPD_PART);
    writeRegister(EPD_BORDERSETTING, 0x04, -1, -1, -1);  //
}

// ************************************************************************************
// READTEMPERATURE - Uses the built-in temperature sensor to read back the actual
// temperature; the value is auto-retrieved at the beginning of an image update. Its
// not that good, accuracy seems to be around ±2C, better useful for playing around
// rather than professional temp monitoring.
// ************************************************************************************
uint8_t PL_microEPD::readTemperature() {
    uint8_t temp;
    digitalWrite(cs, LOW);
    SPI.transfer(EPD_REGREAD | 0x08);
    temp = SPI.transfer(0xFF);
    digitalWrite(cs, HIGH);
    waitForBusyInactive(EPD_TMG_SRT);
    return temp;
}

// PRIVATE

// ************************************************************************************
// POWERON - Activates the defined high voltages needed to update the screen. The
// command should always be called before triggering an image update.
// ************************************************************************************
void PL_microEPD::powerOn() {
    waitForBusyInactive(EPD_TMG_SRT);
    switch (_EPDsize) {
        case 11:
            writeRegister(EPD_SETRESOLUTION, 0, 239, 0, 147);
            break;
        case 14:
            writeRegister(EPD_SETRESOLUTION, 0, 0xEF, 0, 0x9F);
            break;
        case 21:
            writeRegister(EPD_SETRESOLUTION, 0, 239, 0, 159);
            break;
        case 31:
            writeRegister(EPD_SETRESOLUTION, 0, 239, 0, 159);
    }
    writeRegister(EPD_TCOMTIMING, 0x67, 0x55, -1, -1);
    writeRegister(EPD_POWERSEQUENCE, 0x00, 0x00, 0x00, -1);
    writeRegister(EPD_POWERCONTROL, 0xD1, -1, -1, -1);
    waitForBusyInactive(EPD_TMG_SR2);
    delay(10);                              // This value can still be optimized!
}

// ************************************************************************************
// POWEROFF - Deactivates the high voltages needed to update the screen. The
// command should always be called after triggering an image update.
// ************************************************************************************
void PL_microEPD::powerOff() {
    writeRegister(EPD_POWERCONTROL, 0xD0, -1, -1, -1);
    waitForBusyInactive(EPD_TMG_SR2);
    writeRegister(EPD_POWERCONTROL, 0xC0, -1, -1, -1);
    waitForBusyInactive(EPD_TMG_SRT);
}


// ************************************************************************************
// WRITEBUFFER - Sends the content of the memory buffer to the UC8156 driver IC.
// ************************************************************************************
void PL_microEPD::writeBuffer(){
    switch (_EPDsize) {
        case 11:
            writeRegister(EPD_PIXELACESSPOS, 0, 147, -1, -1);		    
            break;
        case 14:
            writeRegister(EPD_PIXELACESSPOS, 0, 0x9f, -1, -1);		    
            break;
        case 21:
            writeRegister(EPD_PIXELACESSPOS, 0, 0, -1, -1);	
            break;
        case 31:
            writeRegister(EPD_PIXELACESSPOS, 0, 0x9b, -1, -1);	
    }    
    digitalWrite(cs, LOW);
    SPI.transfer(0x10);
    if (_EPDsize==31)
        for (int i=0; i < _buffersize; i++) 
            SPI.transfer(buffer2[i]);
    else
        for (int i=0; i < _buffersize; i++) 
            SPI.transfer(buffer[i]);
    digitalWrite(cs, HIGH);
    waitForBusyInactive(EPD_TMG_SRT);
}


// ************************************************************************************
// WRITE REGISTER - Sets register ADDRESS to value VAL1 (optional: VAL2, VAL3, VAL4)
// ************************************************************************************
<<<<<<< HEAD
void PL_microEPD::writeRegister(uint8_t address, int16_t val1, int16_t val2, 
    int16_t val3, int16_t val4) {
    digitalWrite(cs, LOW);
    SPI.transfer(address);
    if (val1!=-1) SPI.transfer((byte)val1);
    if (val2!=-1) SPI.transfer((byte)val2);
    if (val3!=-1) SPI.transfer((byte)val3);
    if (val4!=-1) SPI.transfer((byte)val4);
=======
void PL_microEPD::writeRegister(char address, char val1, signed short val2, signed short val3, signed short val4) {
    digitalWrite(cs, LOW);
    SPI.transfer(address);
    SPI.transfer(val1);
    if (val2>-1) SPI.transfer(val2 & 0xFF);
    if (val3>-1) SPI.transfer(val3 & 0xFF);
    if (val4>-1) SPI.transfer(val4 & 0xFF);
>>>>>>> origin/master
    digitalWrite(cs, HIGH);
    waitForBusyInactive(EPD_TMG_SR2);
}

// ************************************************************************************
// READREGISTER - Returning the value of the register at the specified address
// ************************************************************************************
byte PL_microEPD::readRegister(char address){
    byte data;
    digitalWrite(cs, LOW);                                      
    SPI.transfer(address | EPD_REGREAD);
    data = SPI.transfer(0xFF);                         
    digitalWrite(cs, HIGH);
    waitForBusyInactive(EPD_TMG_SRT);
    return data;                                        // can be improved
}

// ************************************************************************************
// GETEPDSIZE - Returns the size of the attached display diagonal, e.g. 11 is 
// equivalent to to a 1.1" EPD, 21 correpsonds to 2.1" and 31 is equal to 3.1" EPD size
// ************************************************************************************
byte PL_microEPD::getEPDsize(){
    byte data;
    writeRegister(EPD_PROGRAMMTP, 0x02, -1, -1, -1);    // Set MTP2 as active
    writeRegister(EPD_MTPADDRESSSETTING, 0xF2, 0x04, -1, -1); 
                                                        // Set MTP address to 0x04F0h
    data = readRegister(0x43);                          // Read one dummy byte
    data = readRegister(0x43);                          // Read one byte

    if (data==49) {
        data = readRegister(0x43);                      
        if (data==49)                                   // 1.1" detected
            return 11;
        else 
            return 14;                                  // 1.4" detected
    } 
       
    if (data==50)                                       // 2.1" detected
        return 21;
        
    if (data==51)                                       // 3.1" detected
        return 31;
        
    return 99;
}


// ************************************************************************************
// WHITE ERASE - Triggers two white updates to erase the screen and set back previous
// ghosting. Recommended after each power cycling.
// ************************************************************************************
void PL_microEPD::WhiteErase() {
    clear();
    update();
    invert();
    update();
    invert();
    update();
}


// ************************************************************************************
// WAITFORBUSYINACTIVE - Sensing to ‘Busy’ pin to detect the UC8156 driver status.
// Function returns only after driver IC is free again for listening to new commands.
// ************************************************************************************
void PL_microEPD::waitForBusyInactive(int duration){
    if (busy==-1) 
        delay(duration);
    else 
        while (digitalRead(busy) == LOW) {}
}

// ************************************************************************************
// DEEPSLEEP - Putting the UC8156 in deep sleep mode with less than 1µA current @3.3V.
// Reset pin toggling needed to wakeup the driver IC again.
// ************************************************************************************
void PL_microEPD::deepSleep(void) {
    writeRegister(0x21, 0xff, 0xff, 0xff, 0xff); 
}

// ************************************************************************************
// BOSCH BMA250E **********************************************************************
// ************************************************************************************

BO_BMA250::BO_BMA250(uint8_t _cs2) {
    cs2     = _cs2;             //BMA250 accelerometer
}

// ************************************************************************************
// BEGIN - Initialising the Accelerometer BMA250E
// ************************************************************************************
void BO_BMA250::begin() {
    pinMode(cs2, OUTPUT);
    digitalWrite(cs2, LOW);
    SPI.transfer(0x0F);     // Set g
    SPI.transfer(ACC_GSEL);
    digitalWrite(cs2, HIGH);
    digitalWrite(cs2, LOW);
    SPI.transfer(0x10);     // Set bandwith
    SPI.transfer(ACC_BW);
    digitalWrite(cs2, HIGH);
}

// ************************************************************************************
// ACTIVATETAPONINT1 - SETS INT1 HIGH for 1s if a Finger TAP was detected
// ************************************************************************************
void BO_BMA250::activateTapOnInt1() {
    digitalWrite(cs2, LOW);
    SPI.transfer(0x16);     // Enable single tap interrupt
    SPI.transfer(0x20);
    digitalWrite(cs2, HIGH);
    digitalWrite(cs2, LOW);
    SPI.transfer(0x19);     // Map single tap interrupt to Int1 pin
    SPI.transfer(0x20);
    digitalWrite(cs2, HIGH);

    digitalWrite(cs2, LOW);
    SPI.transfer(0x2B);     // Adjust tab sensitivity (01..1F, the lower the more sensitive)
    SPI.transfer(0x01);
    digitalWrite(cs2, HIGH);

    digitalWrite(cs2, LOW);
    SPI.transfer(0x21);     // Set Interrupt mode = temporary or latched (0x0F)
    SPI.transfer(0x0F);
    digitalWrite(cs2, HIGH);
}


// ************************************************************************************
// CLEARLATCHEDINT - As the name suggests the latched INT1 is cleared by this function
// ************************************************************************************
void BO_BMA250::clearLatchedInt1() {
    digitalWrite(cs2, LOW);
    SPI.transfer(0x21);     // Clear latched Int1
    SPI.transfer(0x80);
    digitalWrite(cs2, HIGH);
}


// ************************************************************************************
// READACCCEL - Sensing the device position in space by returning X, Y, Z coordinates.
// ************************************************************************************
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

void BO_BMA250::deepSuspend() {
    digitalWrite(cs2, LOW);
    //SPI.transfer(0x11|0x80); 
    SPI.transfer(0x11); 
    SPI.transfer(0x20); 
    digitalWrite(cs2, HIGH);
}