#ifndef BO_BMA250_h
#define BO_BMA250_h

#define ACC_GSEL    0x03    //0x03 - 2g, 0x05 - 4, 0x08 - 8g, 0x0C - 16g
#define ACC_BW      0x08    //7.81Hz bandwith

class BO_BMA250 {
    
    public:
        BO_BMA250(uint8_t _cs2);
        void initializeBMA250(void);
        void activateTapOnInt1(void);
        void readAccel(void);
    
    private:
        int x, y, z, temp;
        int cs2;
};

#endif
