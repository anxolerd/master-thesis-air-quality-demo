#ifndef RGBLED_HH
#define RGBLED_HH

#include <Arduino.h>

class RGBLed {
    public:
        RGBLed(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
        void begin();
        void red();
        void green();
        void blue();
        void cyan();
        void magenta();
        void yellow();
        void none();
        void white();
        void color(char COLOR);
        static const char COLOR_RED = 0b001;
        static const char COLOR_GREEN = 0b010;
        static const char COLOR_BLUE = 0b100;
        char getColor();
    private:
        uint8_t _red, _green, _blue;
        char _currentColor;
};

#endif