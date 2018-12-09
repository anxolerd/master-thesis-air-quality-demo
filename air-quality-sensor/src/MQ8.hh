#ifndef MQ8_HH
#define MQ8_HH

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class MQ8 {
    public:
        MQ8(uint8_t aoutPin, uint8_t doutPin);
        void begin();
        int readValue();
        bool isLimitReached();
    private:
        uint8_t _aoutPin, _doutPin;
};

#endif