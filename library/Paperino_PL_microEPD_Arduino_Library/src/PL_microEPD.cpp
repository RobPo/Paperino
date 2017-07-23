/* *****************************************************************************************
PL_MICROEPD.CPP - A Hardware Library for 1.1” E-Paper display (EPD) from Plastic Logic based 
on UC8156 IC. This display uses six GPIOs to communicate (4-wire SPI, reset pin, busy pin).

Created by Robert Poser, May 18th 2017, Dresden/Germany
Released under BSD license (3-clause BSD license), check license.txt for more informations.

We invested time and resources providing this open source code, please support Paperino and 
open source hardware by purchasing this product @Crowdsupply @Paperino @Plasticlogic
***************************************************************************************** */
#include "PL_microEPD.h"

byte buffer[EPD_BUFFER_SIZE];				

PL_microEPD::PL_microEPD(uint8_t _cs, uint8_t _rst, uint8_t _busy) : Adafruit_GFX(EPD_WIDTH, 
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
    pinMode(rst, OUTPUT);
    pinMode(busy, INPUT);
    delay(1);
    digitalWrite(rst, HIGH);				//Triggering a global hardware reset
    waitForBusyInactive();				    //All registers are set to default
    digitalWrite(rst, LOW);
    delay(1);
    digitalWrite(rst, HIGH);
    waitForBusyInactive();
    writeRegister(EPD_PANELSETTING, 0x12, -1, -1, -1);      	
    writeRegister(EPD_DRIVERVOLTAGE, 0x25, 0xff, -1, -1); 	
    writeRegister(EPD_WRITEPXRECTSET, 0, 71, 0, 147);	
    writeRegister(EPD_VCOMCONFIG, 0x00, 0x00, 0x24, 0x07);	
    writeRegister(EPD_BORDERSETTING, 0x04, -1, -1, -1);	 
    writeRegister(EPD_LOADMONOWF, 0x60, -1, -1, -1);		
    writeRegister(EPD_INTTEMPERATURE, 0x0A, -1, -1, -1);	
	
    if (whiteErase) WhiteErase();			//Start with a white refresh if TRUE
    setTextColor(EPD_BLACK);				//Set text color to black as default
    setRotation(1);					        //Set landscape mode as default
}

// ************************************************************************************
// CLEAR - Erases the image buffer and triggers an image update and sets the cursor 
// back to the origin coordinates (0,0).
// ************************************************************************************
void PL_microEPD::clear() {
    for (int i=0; i<EPD_BUFFER_SIZE; i++) {
        buffer[i] = 0xff;
    }
    writeBuffer();
    setCursor(0,0);
}

// ************************************************************************************
// DRAWPIXEL - Draws pixel in the memory buffer at position X, Y with the value of the 
// parameter color (2 bit value).
// ************************************************************************************
void PL_microEPD::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= _width || (y < 0) || (y >= _height))) return;  
    
    y=y+3;
    if (color < 4) {
    	uint16_t byteIndex = x/4 + (y) * nextline;
    	uint8_t pixels = buffer[byteIndex];
    				
    	switch (x%4) {					//2-bit grayscale dot
    		case 0: pixels = (pixels & 0x3F) | ((uint8_t)color << 6); break;	
    		case 1: pixels = (pixels & 0xCF) | ((uint8_t)color << 4); break;	
    		case 2: pixels = (pixels & 0xF3) | ((uint8_t)color << 2); break;	
    		case 3: pixels = (pixels & 0xFC) | (uint8_t)color; break;		
	}
	buffer[byteIndex] = pixels;
    }
}


// ************************************************************************************
// INVERT - Inverts the screen content from black to white and vice versa
// ************************************************************************************
void PL_microEPD::invert() {
    for (int i=0; i<EPD_BUFFER_SIZE; i++) buffer[i] = ~buffer[i];
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
    writeBuffer();
    powerOn();
    if (updateMode==0 || updateMode==1) 
        writeRegister(EPD_PROGRAMMTP, 0x00, -1, -1, -1); 
    else
        writeRegister(EPD_PROGRAMMTP, 0x02, -1, -1, -1); 
    if (updateMode==0) writeRegister(EPD_DISPLAYENGINE, 0x03, -1, -1, -1);    
    if (updateMode==1 || updateMode==2) writeRegister(EPD_DISPLAYENGINE, 0x07, -1, -1, -1);    
    waitForBusyInactive();
    powerOff();
}



// ************************************************************************************
// SETROTATION - Let’s you define the display orientation. If set to “1” the landscape
// mode is select (default), if set to “2” the display is set to portrait mode.
// ************************************************************************************
void PL_microEPD::setRotation(uint8_t o) {
    clear();
    if (o==1) {
        nextline = EPD_HEIGHT/4;			//Landscape mode (default)
        writeRegister(EPD_DATENTRYMODE, 0x07, -1, -1, -1);  
        _width  = HEIGHT;
        _height = WIDTH;
    }
    if (o==2) {
        nextline = EPD_WIDTH/4;			//Portrait mode 
        writeRegister(EPD_DATENTRYMODE, 0x02, -1, -1, -1);  
        _width  = WIDTH;
        _height = HEIGHT;
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
    
    for (int offset = 0; offset < text.length(); offset++) {
        String t = "";
        for (int i = 0; i < cutout; i++)
            t += text.charAt((offset + i));
        setCursor(temp_x, temp_y); 
        print(t);
        update(EPD_UPD_MONO);
        clear();
        #ifdef defined(PLATFORM_ID)    // If Particle used, keep cloud connection alive
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
    if (color==3) writeRegister(EPD_BORDERSETTING, 0xF7, -1, -1, -1);	 // 
    if (color==0) writeRegister(EPD_BORDERSETTING, 0x07, -1, -1, -1);	 // 
    update(EPD_UPD_PART);
    writeRegister(EPD_BORDERSETTING, 0x04, -1, -1, -1);	 // 
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
    waitForBusyInactive();
    return temp;
}

// PRIVATE

// ************************************************************************************
// POWERON - Activates the defined high voltages needed to update the screen. The 
// command should always be called before triggering an image update.
// ************************************************************************************
void PL_microEPD::powerOn() {
    waitForBusyInactive();
    writeRegister(EPD_SETRESOLUTION, 0, 239, 0, 147);			
    writeRegister(EPD_TCOMTIMING, 0x67, 0x55, -1, -1);			
    writeRegister(EPD_POWERSEQUENCE, 0x00, 0x00, 0x00, -1);		
    writeRegister(EPD_POWERCONTROL, 0xD1, -1, -1, -1);
    waitForBusyInactive();
    delay(10);
}

// ************************************************************************************
// POWEROFF - Deactivates the high voltages needed to update the screen. The 
// command should always be called after triggering an image update.
// ************************************************************************************
void PL_microEPD::powerOff() {
    writeRegister(EPD_POWERCONTROL, 0xD0, -1, -1, -1);
    waitForBusyInactive();
    writeRegister(EPD_POWERCONTROL, 0xC0, -1, -1, -1);
}


// ************************************************************************************
// WRITEBUFFER - Sends the content of the memory buffer to the UC8156 driver IC.
// ************************************************************************************
void PL_microEPD::writeBuffer(){
    writeRegister(EPD_PIXELACESSPOS, 0, 147, -1, -1);		
    digitalWrite(cs, LOW);
    SPI.transfer(0x10);
	for (int i=0; i < EPD_BUFFER_SIZE; i++) SPI.transfer(buffer[i]); 
    digitalWrite(cs, HIGH);
    waitForBusyInactive();
}


// ************************************************************************************
// WRITE REGISTER - Sets register ADDRESS to value VAL1 (optional: VAL2, VAL3, VAL4)
// ************************************************************************************
void PL_microEPD::writeRegister(char address, char val1, char val2, char val3, char val4) {
    digitalWrite(cs, LOW);
    SPI.transfer(address);
    SPI.transfer(val1);
    if (val2>-1) SPI.transfer(val2);
    if (val3>-1) SPI.transfer(val3);
    if (val4>-1) SPI.transfer(val4);
    digitalWrite(cs, HIGH);
    waitForBusyInactive();
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
    clear();
    update();
}


// ************************************************************************************
// WAITFORBUSYINACTIVE - Sensing to ‘Busy’ pin to detect the UC8156 driver status.
// Function returns only after driver IC is free again for listening to new commands.
// ************************************************************************************
void PL_microEPD::waitForBusyInactive(){
    while (digitalRead(busy) == LOW) {}
}